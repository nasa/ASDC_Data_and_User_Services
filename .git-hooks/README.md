# Notebook Pre-commit Hooks

Automated validation for Jupyter notebooks to ensure consistency and catch errors early.

## Quick Start

```bash
# Install and enable
uv tool install pre-commit   # pre-commit is not a project dependency; install it separately
uv sync                      # install Python dependencies
pre-commit install

# Hooks now run automatically on commit
git add TEMPO/my-notebook.ipynb
git commit -m "Add tutorial"

# Or run manually
pre-commit run --all-files                                       # every hook, as configured
uv run python .git-hooks/check-notebook-headers.py --all-notebooks
uv run python .git-hooks/check-notebook-imports.py --all-notebooks
```

Notebooks live in the mission folders (`TEMPO/`, `PREFIRE/`, and so on). Use your
notebook's path in place of `TEMPO/my-notebook.ipynb`.

## Where the Checks Run

| Script | On commit (pre-commit) | On pull requests to `main` (`validate-pr.yml`) |
|--------|------------------------|------------------------------------------------|
| `check-notebook-headers.py` | Blocks the commit | Blocks the PR |
| `check-notebook-imports.py` | Advisory (`--warn-only`); never blocks | Advisory (`--warn-only`); never blocks |
| `check-unused-dependencies.py` | Not run | Not run |

The two differ in which notebooks they check:

- **pre-commit** passes only the staged `.ipynb` files to each script. It skips any
  file under an `additional_drafts/` or `_archive/` directory, using its own
  `exclude` pattern.
- **`validate-pr.yml`** runs both scripts with `--all-notebooks --exclude-archived
  --exclude-additional-drafts`. These flags skip any path that *contains* `archived`,
  `_archive`, or `additional_drafts` (see [Common Options](#common-options)).

The hook definitions are in [`.pre-commit-config.yaml`](../.pre-commit-config.yaml),
under the `repo: local` entries. The CI steps are in
[`.github/workflows/validate-pr.yml`](../.github/workflows/validate-pr.yml). This
README does not copy either file, so check them for the exact settings.

## Hooks

### check-notebook-headers.py

Validates required documentation sections. Each element can appear in any Markdown
cell. Headings may use `##` or `###` and may end with a colon.

**Checks for:**
- `---\ndate: last-modified\n---` frontmatter
- `Summary` or `Overview` heading
- `Prerequisites` heading
- `Notebook Author/Affiliation`, `Notebook Author`, or `Author` heading

A notebook with no Markdown cells fails the check.

**Usage:**
```bash
# Default: check staged notebooks
uv run python .git-hooks/check-notebook-headers.py

# Check all notebooks
uv run python .git-hooks/check-notebook-headers.py --all-notebooks

# Exclude archived/drafts (what CI runs)
uv run python .git-hooks/check-notebook-headers.py --all-notebooks --exclude-archived --exclude-additional-drafts

# Verbose output
uv run python .git-hooks/check-notebook-headers.py --all-notebooks -v
```

### check-notebook-imports.py

Checks that every package a notebook imports is declared as a dependency. The script
parses code cells and ignores lines that start with `%` or `!`. A top-level import
name is allowed if it is one of these:

- a package in `[project] dependencies` in `pyproject.toml`, or an import name that
  `get_import_aliases()` in `notebook_utils.py` maps to one of those packages
- a standard library module
- a name in `ALLOWLIST` in `check-notebook-imports.py` (local helper modules and
  packages that cannot be declared)

This check is advisory on commit and in CI. Without `--warn-only`, the script exits
with status 1 when it finds unauthorized imports.

**Usage:**
```bash
# Check all notebooks
uv run python .git-hooks/check-notebook-imports.py --all-notebooks

# Fix unauthorized imports
uv add package-name  # Add to dependencies
```

### check-unused-dependencies.py

Identifies dependencies declared in `pyproject.toml` but not imported in any notebook.

**This script is manual only.** Neither pre-commit nor CI runs it. It always exits 0,
so treat its output as a list to review, not a failure. Unlike the other two scripts:

- It always scans every notebook. It ignores file arguments, `--all-notebooks`, and
  `--warn-only`.
- `--exclude-archived` skips only paths containing `archived`, so notebooks in
  `_archive/` are still scanned.
- `--include-optional` adds `[project.optional-dependencies]` groups to the check.
  `pyproject.toml` defines no such groups, so the flag currently has no effect.
- Infrastructure packages listed in `INFRASTRUCTURE_PACKAGES` (such as `jupyter` and
  `ipykernel`) are never reported.

**Usage:**
```bash
uv run python .git-hooks/check-unused-dependencies.py --exclude-archived --exclude-additional-drafts
```

## Common Options

These flags apply to `check-notebook-headers.py` and `check-notebook-imports.py`.

| Flag | Description |
|------|-------------|
| `FILE ...` | Check only these notebooks. Takes precedence over `--all-notebooks`; the `--exclude-*` flags do not apply. pre-commit uses this form. |
| (no files, no flag) | Check staged notebooks |
| `--all-notebooks` | Check every notebook under the current directory, skipping hidden directories |
| `--exclude-archived` | With `--all-notebooks`, skip paths containing `archived` or `_archive` |
| `--exclude-additional-drafts` | With `--all-notebooks`, skip paths containing `additional_drafts` |
| `-v, --verbose` | Also list passing files and show error details |
| `-q, --quiet` | Print nothing; only the exit status reports the result |
| `--warn-only` | Report issues but always exit 0 |

## Common Workflows

**Audit all current notebooks (same as CI):**
```bash
uv run python .git-hooks/check-notebook-headers.py --all-notebooks --exclude-archived --exclude-additional-drafts
uv run python .git-hooks/check-notebook-imports.py --all-notebooks --exclude-archived --exclude-additional-drafts --warn-only
```

**Check specific notebooks:**
```bash
uv run python .git-hooks/check-notebook-headers.py TEMPO/my-notebook.ipynb
```

## Troubleshooting

**"No notebooks to check"**
- Stage notebooks with `git add` (default mode checks staged files)
- Use `--all-notebooks` to check all notebooks

**"Unauthorized imports"**
- Add package to `pyproject.toml`: `uv add package-name`
- If the import name differs from the package name, add a mapping in
  `get_import_aliases()` in `notebook_utils.py` (for example, `sklearn` →
  `scikit-learn`)
- If it is a local helper module, or a package that cannot be installed with pip,
  add it to `ALLOWLIST` in `check-notebook-imports.py`

**"Parse error"**
- Open and re-save notebook in Jupyter
- Verify notebook is valid JSON

**Hook doesn't run on commit**
- Install hooks: `pre-commit install`
- Verify `.pre-commit-config.yaml` exists

## Configuration

**Hook wiring**: see [`.pre-commit-config.yaml`](../.pre-commit-config.yaml) and
[`.github/workflows/validate-pr.yml`](../.github/workflows/validate-pr.yml).

**Modify required sections** (`check-notebook-headers.py`):
```python
REQUIRED_ELEMENTS = {
    "section_name": {
        "patterns": [r"regex_pattern"],
        "description": "Human readable name",
    },
}
```

**Modify allowed packages**: run `uv add package-name`, which updates
`[project] dependencies` in `pyproject.toml`. To allow imports that are not
packages, edit `ALLOWLIST` in `check-notebook-imports.py`.

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

2. Add to `.pre-commit-config.yaml`, and to `.github/workflows/validate-pr.yml` if
   it should also run on pull requests

3. Document in this README

## Support

- **Pre-commit docs**: https://pre-commit.com/
