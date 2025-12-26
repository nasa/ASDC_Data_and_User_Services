# Notebook Pre-commit Hooks

Automated validation for Jupyter notebooks to ensure consistency and catch errors early.

## Quick Start

```bash
# Install and enable
uv sync --extra dev
pre-commit install

# Hooks now run automatically on commit
git add notebooks/my-notebook.ipynb
git commit -m "Add tutorial"

# Or run manually
python .git-hooks/check-notebook-headers.py --all-notebooks
python .git-hooks/check-notebook-imports.py --all-notebooks
```

## Hooks

### check-notebook-headers.py

Validates required documentation sections.

**Checks for:**
- `---\ndate: last-modified\n---` header
- `## Summary` or `## Overview` section
- `## Prerequisites` section  
- `## Author` section

**Usage:**
```bash
# Default: check staged notebooks
python .git-hooks/check-notebook-headers.py

# Check all notebooks
python .git-hooks/check-notebook-headers.py --all-notebooks

# Exclude archived/drafts
python .git-hooks/check-notebook-headers.py --all-notebooks --exclude-archived --exclude-additional-drafts

# Verbose output
python .git-hooks/check-notebook-headers.py --all-notebooks -v
```

### check-notebook-imports.py

Validates imports match `pyproject.toml` dependencies.

**Usage:**
```bash
# Check all notebooks
python .git-hooks/check-notebook-imports.py --all-notebooks

# Fix unauthorized imports
uv add package-name  # Add to dependencies
```

### check-unused-dependencies.py

Identifies dependencies declared in `pyproject.toml` but not imported in any notebook.

**Usage:**
```bash
# Check main dependencies
python .git-hooks/check-unused-dependencies.py

# Include optional dependency groups
python .git-hooks/check-unused-dependencies.py --include-optional

# Exclude archived notebooks
python .git-hooks/check-unused-dependencies.py --exclude-archived

## Common Options

| Flag | Description |
|------|-------------|
| `--all-notebooks` | Check all notebooks (not just staged) |
| `--exclude-archived` | Skip `archived/` directories |
| `--exclude-additional-drafts` | Skip `additional_drafts/` directories |
| `-v, --verbose` | Show all results including passes |
| `-q, --quiet` | Only show errors |

## Common Workflows

**Audit all current notebooks:**
```bash
python .git-hooks/check-notebook-headers.py --all-notebooks --exclude-archived --exclude-additional-drafts
python .git-hooks/check-notebook-imports.py --all-notebooks --exclude-archived --exclude-additional-drafts
```

**Check specific notebooks:**
```bash
python .git-hooks/check-notebook-headers.py notebooks/tutorial.ipynb
```

**CI/CD integration:**
```bash
# Add to GitHub Actions
python .git-hooks/check-notebook-headers.py --all-notebooks --exclude-archived
```

## Troubleshooting

**"No notebooks to check"**
- Stage notebooks with `git add` (default mode checks staged files)
- Use `--all-notebooks` to check all notebooks

**"Unauthorized imports"**
- Add package to `pyproject.toml`: `uv add package-name`
- Check for import aliases (e.g., `sklearn` → `scikit-learn`)

**"Parse error"**
- Open and re-save notebook in Jupyter
- Verify notebook is valid JSON

**Hook doesn't run on commit**
- Install hooks: `pre-commit install`
- Verify `.pre-commit-config.yaml` exists

## Configuration

**Pre-commit setup** (`.pre-commit-config.yaml`):
```yaml
repos:
  - repo: local
    hooks:
      - id: check-notebook-headers
        name: Check notebook headers
        entry: python .git-hooks/check-notebook-headers.py
        language: system
        types: [jupyter]
        pass_filenames: true
      
      - id: check-notebook-imports
        name: Check notebook imports
        entry: python .git-hooks/check-notebook-imports.py
        language: system
        types: [jupyter]
        pass_filenames: false
```

**Modify required sections** (`check-notebook-headers.py`):
```python
REQUIRED_ELEMENTS = {
    "section_name": {
        "patterns": [r"regex_pattern"],
        "description": "Human readable name",
    },
}
```

**Modify allowed packages** (`pyproject.toml`):
```toml
[project]
dependencies = ["package>=1.0.0"]

[project.optional-dependencies]
specialized = ["specialized-package>=1.0.0"]
```

## Adding New Hooks

1. Create script in `.git-hooks/`:
```python
from notebook_utils import create_base_parser, get_notebooks_from_args, HookResult

def main():
    parser = create_base_parser("What this checks")
    args = parser.parse_args()
    notebooks, mode = get_notebooks_from_args(args)
    # Your validation logic
```

2. Add to `.pre-commit-config.yaml`

3. Document in this README

## Support

- **ChatGSFC support**: gsfc-dl-caio@mail.nasa.gov
- **Pre-commit docs**: https://pre-commit.com/