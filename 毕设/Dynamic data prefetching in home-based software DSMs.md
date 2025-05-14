Method: Records the history of INV and GETP operations for each cached page and maintains a sequential string of INV and GETP for that page. This access history string is checked for periodicity when the page is invalidated on a lock or barrier. A prefetching tag is set if the periodicity check implies that GETP will be the next operation in the string. After the lock or barrier, prefetching messages are sent for all tagged cache pages. Multiple prefetches are merged into the same message if they are to the same home host. With this prefetching scheme, the long remote access latency can be overlapped with other operations because the prefetching processor can go ahead without waiting for prefetched pages to be sent back. Besides, the number of remote access messages is reduced because multiple pages may be prefetched in the same message.

方法：记录每个缓存页的 INV 和 GETP 操作的历史，维护该页的INV和GETP的顺序串。当页在 Lock 或  Barrier 处被失效时，该访问历史串将被周期性检查。如果周期检查显式该串的下一个操作是 GETP，将设置预取标签（prefetching tag）。在 lock 或 barrier 之后，将发送所有标记的缓存页的预取消息。如果它们被发往同一主机，多个预取将被合并至同一消息。通过该预取方案，长远程访问延迟可以与其他操作重叠，因为预取处理器可以不等待预取页面返回就继续进行。此外，由于在同一消息中可能预取了多个页面，因此减少了远程访问消息的数量

---
JIAJIA 实现了域一致性内存模型。利用多写者技术以降低假共享。在 JIAJIA 中，缓存页的一致性将通过保存在锁上的写通知（write-notices）来维护。

- 在释放锁时（On a Release），释放者将该区间中修改的缓存页面与它们的 twins 进行比较，以产生 diffs。这些 diffs 被发往它们的家节点。在所有 diffs 被应用于 home 页后，将向锁管理器发一条消息指示释放该锁。该区间的写通知（Write-notices）被打包到释放消息以通知该区间中修改的页。
- 在获取锁时（On an acquire），获取者向锁管理器发送锁获取请求。获取者暂停直到被授权请求的锁。当锁管理器授权锁时，将打包该锁相关的写通知到授权消息中。获取者收到该授权消息后，它将失效所有写通知指示的页对应的缓存页。
> Barrier 可被视为 `unlock` 和 `lock` 的结合。所有锁的所有写通知在同步时将被清除。


在读不命中（read miss）时，将从页的 Home 处获取错误页面（fault page）并至于 RO 状态。在写不命中（write miss）时，如果要写入的页不存在或在 cache 中处于 INV 状态，它将从页的 Home 处取回并至于 RW 状态。如果要写入的页在 cache 中处于 RO 状态，状态将被转换为 RW。
该页对应的写通知将被记录，该页的副本（twin）将被创建。在替换 cache 中的页时，如果它处于 RW状态，替换的页将被写回到它的 Home 主机。

上述描述了基于锁的缓存一致性协议不包含任何优化，下面是基于基础协议的一些优化：
- 单写检测：在基础协议中，如果获取的锁中包含的写通知指示某页已经被修改，对应的缓存页在获取锁时将被失效。但是，如果前述修改是由获取者自身修改的，那么失效是没必要的，因为此修改早已为获取者可见。此外，如果一页仅被其 Home 节点修改，并且不存在该页的缓存副本，则没有必要将其对应的写通知在释放锁或遇到屏障时发送给锁管理器。
- Incarnation Number Technique：版本号技术的目的是消除锁上不必要的失效（invalidation）。通过该优化，每个锁与一个版本号相关联，当锁传递时该编号增加。处理器在获取锁时记录锁的当前版本号。当处理器再次获取锁时，它在获取请求时发送该版本号给锁管理器。收到该版本号后，锁管理器知道哪一个写通知在前一次锁授权中已经被送往获取方，因此这一次将它们排除在发往获取方的写通知之外。
- Lazy Home Page Write Detection：通常的 home page 写检测的方法是在同步间隔开始时对主页写保护，以便通过页面错误检测到对主页的写操作。懒惰的 home page 写检测则延迟写保护到页面首次在间隔内被获取，这样未被远程处理器缓存的主页就不需要写保护。
- 写向量技术：写向量优化的动机是 homeless 协议中仅在页错误时获取 diffs 。它通过将页面划分为块（blocks）来避免获取导致页错误的整个页面，而只获取那些在页错误时对处理器而言是脏的块。为每个共享页维护一个写向量表，以记录每个处理器自处理器上次获取页面以来已被修改的块。
- Home迁移：Home 迁移优化自适应地将页面的 Home 迁移到最频繁写入该页面的处理器，以在间隔结束时减少 diffs 传播开销，因为在基于的 home 协议中写入主页面不会产生 twin 和 diff 。在 Home 迁移方案中，在两个屏障之间仅由一个处理器写入的页被屏障管理器识别，这些页的 Home 被迁移到单个写入处理器，迁移信息是附加在屏障消息上的，不需要为迁移进行额外的通信。

---
通常，预取方法决定通过**回顾执行过程中的以前内存访问事件**来进行预取。预取方案的一个关键点是在正确的时间预取数据。过于保守的预取算法可能会限制预取的效果，而过于激进的算法可能会浪费通信。如果过早预取，那么预取的数据可能在真正使用之前失效。在上面的示例中，对于处理器0以外的处理器，在第一个屏障间隔内预取共享阵列是不明智的。在实际应用程序中，访问历史字符串可能比上一个示例中更复杂。

---
对于缓存页，在页错误时涉及两种一致性相关的操作：INV 和 GETP。
INV表示应该根据缓存一致性协议传播到本地处理器的远程写，而GETP表示对页面的本地引用。因此，访问页面的访问历史可以用INV和GETP的顺序字符串来表示，称为访问历史串。已缓存页面的访问历史记录字符串将显示该页面的共享行为。


