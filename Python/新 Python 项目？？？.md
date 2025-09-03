1. 在克隆项目后，先查看依赖说明
通常项目根目录会有：
- `requirements.txt`
- `pyproject.toml` / `poetry.lock` （`Poetry` 管理）
- `Pipfile` / `Pipfile.lock` （`Pipenv` 管理）
- `environment.yml`（`conda` 管理）

1. 创建虚拟环境（项目目录下创建 `.venv` 文件夹，然后在 `.gitignore` 中忽略它）
- 使用 `venv` （标准库，自带）
```python
python -m venv .venv
source .venv/bin/activate   # Linux/ macOS
.ven\Scripts\activate       # Windows
```

- 使用 `Poetry` （现代依赖管理工具）
```bash
poetry install
poetry shell
```

- 使用 `conda` （依赖科学计算较多）
```bash
conda create -n myproject python=3.11
conda activate myproject
```

3. 安装依赖
- 如果有 `requirements.txt`:
```bash
pip install -r requirements.txt
```

- 如果有 `pyproject.toml`（`Poetry`）：
```python
poetry install
```

- 如果有 `Pipfile`：
```python
pipenv install
```

4. 固定和导出依赖（保证可复现）
- `Pip`:
```bash
pip freeze > requirements.txt
```

- `Poetry`:
```bash
poetry lock
```

- `Conda`:
```python
conda env export > environment.yml
```