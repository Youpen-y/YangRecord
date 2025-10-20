DVD-RW（Digital Versatile Disc ReWritable）是一种可重复擦写的光盘存储技术。

#### 基本规格
- 容量：4.7 GB（单面单层）
- 物理尺寸：直径 120 mm，厚度 1.2 mm
- 擦写次数：约 1000 次
- 数据保存期：30-100年
- 读取速度：最高 16（21.12 MB/s）
- 写入速度：2x-6x（2.77-8.31 MB/s）

#### 两种工作模式
1. Sequential Recording Mode（顺序记录模式）
- 默认模式，兼容性最好
- 支持多会话
- 只能顺序追加，不能随机覆写
- 类似 CD-RW 的工作方式
数据组织
```
┌─────────────┬─────────────┬─────────────┬──────────┐ 
│ Session 1   │ Session 2   │  Session 3  │  空白区   │ 
│ (已封闭)     │ (已封闭)     │ (已封闭)     │          | └─────────────┴─────────────┴─────────────┴──────────┘
    只读          只读          只读          可追加
```
刻录步骤：
```bash
# 1. 先完全擦除
sudo cdrecord dev=/dev/sr0 blank=all

# 2. 初始化（首次刻录）
mkdir -p ~/dvd_data
cp /path/to/your/files ~/dvd_data # 添加要刻录的文件
sudo growisofs -Z /dev/sr0 -R -J ~/dvd_data/

# 3. 追加新文件
sudo umount /dev/sr0    # 先卸载自动挂载

cp /path/to/more/files ~/dvd_data
sudo growisofs -M /dev/sr0 -R -J ~/dvd_data/
```