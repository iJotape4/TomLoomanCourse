# Python Automation Assignments

Solutions for [`Docs/PythonAutomation_StudyPlan.md`](../../../Docs/PythonAutomation_StudyPlan.md).

## Conventions
- One module per assignment: `m01_banner.py`, `m02_naming_audit.py`, `m03_content_cleanup.py`, … `m10_validators.py`, plus `ci_report.py`.
- Every script exposes a `main()` (or clearly named entry points), and nothing heavy runs at import time. This keeps `reload_all()` and the menus (M6) safe.
- Scripts that modify content default to a **dry run** (`apply=False`).
- Reports go to `Saved/Reports/` (git-ignored with `Saved/`).

## Running
`Content/Python` is on the editor's Python path, so from the Output Log (`Cmd` mode):

```
py "assignments/m02_naming_audit.py"
```

or in `Python` mode:

```python
from assignments import m02_naming_audit; m02_naming_audit.main()
```

Headless (M10):

```
UnrealEditor-Cmd.exe "<path>/TomLoomanCourse.uproject" -run=pythonscript -script="<path>/Content/Python/assignments/ci_report.py" -unattended -nullrhi -stdout
```
