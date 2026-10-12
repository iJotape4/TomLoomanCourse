# Runs automatically when the editor starts (any init_unreal.py on the Python path is executed).
import unreal

from assignments import m01_banner, m02_naming_audit

unreal.log("[Python] TomLoomanCourse Python environment loaded")

#m01_banner.main()
m02_naming_audit.main()
