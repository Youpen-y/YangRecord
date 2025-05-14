```python
import matplotlib.pyplot as plt
from matplotlib.colors import to_hex

# 设置画布
fig, ax = plt.subplots(figsize=(10, 2))

# 打印和显示颜色
for i in range(10):
    color_code = f'C{i}'
    hex_color = to_hex(color_code)
    
    # 输出 HEX 值
    print(f'{color_code}: HEX = {hex_color}')
    
    # 绘图展示颜色
    ax.add_patch(plt.Rectangle((i, 0), 1, 1, color=color_code))
    ax.text(i + 0.5, 0.5, f'{color_code}\n{hex_color}', ha='center', va='center',
            fontsize=10, color='white' if i in [0, 3, 5] else 'black')

# 图像美化
ax.set_xlim(0, 10)
ax.set_ylim(0, 1)
ax.axis('off')
plt.tight_layout()
plt.savefig('default-color.pdf', dpi=300, bbox_inches='tight')
plt.show()

```

![[Matplotlib默认颜色.png]]