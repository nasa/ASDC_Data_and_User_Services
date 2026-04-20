# Data Tutorials and User Services for the ASDC

How-to guides, tutorials, and scripts for working with data from missions
archived and distributed by NASA's
[Atmospheric Science Data Center (ASDC)](https://asdc.larc.nasa.gov/).
The site covers CALIPSO, CERES, DSCOVR, MAIA, MISR, MOPITT, POWER, PREFIRE,
SAGE III-ISS, STAQS, TEMPO, and TOLNet.

**Browse the rendered site: <https://nasa.github.io/ASDC_Data_and_User_Services/>**

*This site is under active, open development. Stay tuned for more and ever-evolving content!*

## Getting started (contributors)

You need [Python 3.10+](https://www.python.org/),
[uv](https://docs.astral.sh/uv/), and
[Quarto](https://quarto.org/docs/get-started/).

```bash
git clone https://github.com/nasa/ASDC_Data_and_User_Services.git
cd ASDC_Data_and_User_Services

uv sync              # install Python dependencies
pre-commit install   # enable local quality checks on commit
quarto preview       # live-preview the site at localhost
```

See the [Contributing Guide](https://nasa.github.io/ASDC_Data_and_User_Services/contributing.html)
for notebook style and content guidelines.

### Environment files

This repo has two environment definitions. They serve different purposes:

| File | Managed by | Purpose |
|------|------------|---------|
| `pyproject.toml` / `uv.lock` | `uv` | **Local development.** All packages needed to run every notebook. Use `uv sync` to install. |
| `environment.yml` | conda (CI only) | **Site builds.** Minimal Jupyter kernel so Quarto can render pre-executed notebooks. You should not need this locally. |

Quarto is configured with `freeze: true`, which means it never re-executes
notebook code during a build. It converts the already-rendered `.ipynb` outputs
straight to HTML. This keeps CI fast and avoids the need for data access
credentials in the build environment.

### Pre-commit hooks

The pre-commit configuration runs on every commit:

- **gitleaks** — secret detection
- **ruff** — Python linting and formatting
- **markdown-link-check** — broken link detection
- **check-notebook-headers** — validates required sections (summary, prerequisites, author)
- **check-notebook-imports** — cross-references imports against `pyproject.toml` (warn-only)

See [`.git-hooks/README.md`](.git-hooks/README.md) for details on the custom
notebook hooks.

## Data files

Notebooks should fetch data programmatically at runtime (e.g., via
`earthaccess` or `harmony-py`) rather than committing data files to the
repository. Large files such as `.nc`, `.hdf`, and `.h5` are not tracked
by git and should stay that way.

## Questions or issues?

Open an [issue](https://github.com/nasa/ASDC_Data_and_User_Services/issues) or
reach out via the [Earthdata Forum](https://forum.earthdata.nasa.gov/).
