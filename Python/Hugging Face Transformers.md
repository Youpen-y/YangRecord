Hugging Face：Datasets（数据集）+ Model（预训练的 ML 模型）+ Spaces（允许在一个有组织的工作区中构建、托管和共享您的 ML 应用程序）
`Transformers` 库提供了用于下载、运行和训练最先进的开源 AI 模型的 API 和工具。涵盖包括自然语言处理、计算机视觉、音频处理、多模态等任务。


核心抽象
- Auto classes （`AutoModel`/`AutoTokenizer`/`AutoConfig` 等）：根据模型 `id` 自动选择正确的模型/分词器类，极大简化迁移与加载。
- `Tokenizer`：有 Python 慢分词器和 Rust 快分词器
- `pipeline` 函数：一行代码做常见人物（情感分析、填空、生成、图像分类、语音识别等）——适合快速原型与演示（见下例）。
```python
from transformers import pipeline

emodel_id = "meta-llama/Llama-3.2-3B-Instruct"
pipe = pipeline(
    "text-generation",
    emodel_id
)

messages = [{"role": "system", "content": "You are LLaMA, answer me in chinese"}]

while True:
    user_prompt = input("😊 你说：")
    if user_prompt.lower() == "exit":
        #print("聊天結束啦，下次再聊喔！👋")
        break
        
    messages.append({"role": "user", "content": user_prompt})
	outputs = pipe( # 呼叫模型生成回應
		messages,
		max_new_tokens=2000,
		pad_token_id=pipe.tokenizer.eos_token_id
	)
    response = outputs[0]["generated_text"][-1]['content'] # 从输出中取出模型生成的回应

    print("🤖 助理说：", response)
    messages.append({"role": "assistant", "content": response})
```


示例：使用 Hugging Face Transformers 库进行文本情绪分析
```python
from transformers import AutoTokenizer, AutoModelForSequenceClassification
import torch

model_name = "distilbert-base-uncased-finetuned-sst-2-english"

tokenizer = AutoTokenizer.from_pretrained(model_name)
model = AutoModelForSequenceClassification.from_pretrained(model_name)

text = "I love programming!"

# 预处理文本
tokens = tokenizer(text, padding=True, truncation=True, return_tensors="pt")

# 模型推理
with torch.no_grad():
    outputs = model(**tokens)
    logits = outputs.logits
    probabilities = torch.softmax(logits, dim=1)
    
# 解释结果
label_ids = torch.argmax(probabilities, dim=1)
labels = ['Negative', 'Positive']
label = labels[label_ids]
print(f"The sentiment is: {label}")
```


示例：
```python
from transformers import AutoTokenizer, AutoModelForCausalLM

model_id = "google/gemma-3-1b-it"   # 或"meta-llama/Llama-3.2-3B-Instruct" or "meta-llama/Llama-3.2-1B-Instruct"

# 准备好 tokenizer 和 model
tokenizer = AutoTokenizer.from_pretrained(model_id)
model = AutoModelForCausalLM.from_pretrained(model_id)

print("Tokens number:", tokenizer.vocab_size) # 查看语言模型有多少 Token 可在“接龙”时选择
print(tokenizer.decode([0, 1, 2, 3, 4, 5])) # 查看多个编号对应的 token
# 将所有 token 保存至文件
with open("tokens.txt", mode="a", encoding="utf-8") as f:
    for token_id in range(tokenizer.vocab_size): #token_id: [0, tokenizer.vocab_size-1]
    token = tokenizer.decode([token_id])
    f.write(f"<{token_id}:<{token}>\n")


# 使用 tokenizer.encode 编码一段文字
text = "大家好"
tokens = tokenizer.encode(text, add_special_tokens=False) # 把 text 中的文字编码成一串 token id， add_special_tokens=False 可以避免加上代表起始的符号
print(text, "->", tokens)

# "good morning" 和 "i am good" 中 good 的编号不一样，原因是 "good" 和 " good"不是同一个 token
print("good morning" ,"->", tokenizer.encode("good morning",add_special_tokens=False))
print("i am good" ,"->", tokenizer.encode("i am good",add_special_tokens=False))

prompt = "1+1=" # 输入提示词
input_ids = tokenizer.encode(prompt, return_tensors="pt") # "pt": PyTorch tensor 格式
outputs = model(input_ids)
# outputs.logits 是模型对输入每个位置、每个 token 的信心分数（还没转成机率）
last_logits = outputs.logits[:, -1, :]  # 得到一个 token 接在 prompt 后面的信息分数
probabilities = torch.softmax(last_logits, dim=-1) # softmax 将原始信心分数转成 0~1 之间的机率值

# 打印出机率最高的前 top_k 名 token
top_k = 10
top_p, top_indices = torch.topk(probabilities, top_k)
print(f"机率最高的前 {top_k} 名 token::")
for i in range(top_k):
    token_id = top_indices[0][i].item() # 取得第 i 名的 token ID
    proability = top_p[0][i].item() # 对应的机率
    token_str = tokenizer.decode(token_id) # 将 token ID 解码为字符串
    print(f"Token ID: {token_id}, Token: '{token_str}', 机率: {probability:.4f}")
    
# 连续产生多个 token
prompt = "你是谁"
length = 20

for t in range(length):
    print("现在的 prompt 是:", prompt)
    input_ids = tokenizer.encode(prompt, return_tensors="pt")
    
    # 使用模型 model 产生下一个 token
    outputs = model(input_ids)
    last_logits = outputs.logits[:, -1, :]
    probabilities = torch.softmax(last_logits, dim=-1)
    #top_p, top_indices = torch.topk(probabilities, 1)
    #token_id = top_indices[0][0].item() # 取得第 1 名的 token ID（取机率最高的 token）
    token_id = torch.multinomial(probabilities, num_samples=1).squeeze() # 换成根据概率来掷骰子
    token_str = tokenizer.decode(token_id)
    print("下一个 token 是:", token_str)
    
    prompt = prompt + token_str # 将新产生的 token 接回 prompt，作为下一轮的输入
    
# 使用 model.generate 做文字接龙
prompt = "你是谁?"
input_ids = tokenizer.encode(prompt, return_tensors="pt")

outputs = model.generate(
	input_ids,     # prompt 的 token IDs
	max_length=20, # 最长输出的 token 数（包含原本的 prompt)
	do_sample=True, # 启用随机抽样（不是永远机率最高）
	top_k=3,      # 每次只从机率最高的前 top-k 个中抽，如果 top_k = 1,变成选机率最高的
	pad_token_id=tokenizer.eos_token_id,
	attention_mask=torch.ones_like(input_ids) 
)
# 除了这里采用的只从 top_k 中选择的方式外，还有许多根据机率选择 token 的策略
generated_text = tokenozer.decode(outputs[0]) # with skip_special_tokens=True 跳过特殊 token
print("生成的文字是:\n", generated_text)

# 添加 chat 模板，在输入的 prompt 前加一些模板
prompt_with_chat_template = "使用者说:" + prompt + "\nAI回答:" + "hahaha"
input_ids = tokenizer.encode(prompt_with_chat_template, return_tensors="pt")

# 使用官方的 chat 模板
prompt = "你是誰?"
print("現在的 prompt 是:", prompt)
messages = [
    {"role": "system", "content": "你的名字是小助手"},  # 自己添加 system prompt
    {"role": "user", "content": prompt},
]
print("現在的 messages 是:", messages)
input_ids = tokenizer.apply_chat_template( #不只加上Chat Template，順便幫你 encode 了
    messages,
    add_generation_prompt=True,
    # add_generation_prompt=True 表示在最後一個訊息後加上一個特殊的 token (e.g., <|assistant|>)
    # 這會告訴模型現在輪到它回答了。
    return_tensors="pt"
)
```

示例：聊天示例
```python
# 存放整个聊天历史信息的 list
messages = []

# 一开始设定角色
messages.append({"role": "system", "content": "你的名字是 Llama, 简短回答问题"})

while True:
    # 用户输入消息
    user_prompt = input("Please input: ")
    
    if user_prompt.lower() == "exit":
        print("Bye~, see you next time!")
        break
    # 将用户消息加进对话记录    
    messages.append({"role": "user", "content": user_prompt})
    
    # 将历史消息转换为模型可以理解的格式
    input_ids = tokenizer.apply_chat_template(
	    messages,
	    add_generation_prompt=True, # 会在消息后门面加入一个特殊标记(<|assistant|>)
	    return_tensors="pt"
    )
    
    # 生成模型的问题
    outputs = model.generate(
	    input_ids,
	    max_length=2000,
	    do_sample=True,
	    top_k=3,
	    pad_token_id=tokenizer.eos_token_id,
	    attention_mask=torch.ones_like(input_ids)
    )
    
    # 将模型的输出转换为文字
    generated_text = tokenizer.decode(outputs[0], skip_special_tokens=False)
    
    # 从生成的结果中取出模型真正的回复内容（去除特殊 token）
    response = generated_text.split("<|end_header_id|>")[-1].split("<|eot_id|>")[0].strip()
    
    print("🤖 助手說：", response)
    
    # 将模型回复消息加进对话记录
    messages.append({"role": "assistant", "content": response})
```