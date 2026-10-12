import unreal

def list_menu(num=1000):
    menu_list = set()
    for i in range(num):
        obj = unreal.find_object(None, "/Engine/Transient.ToolMenus_0:RegisteredMenu_%s" % i ) 
        
        if not obj:
            # UE4 compatibility code
            obj = unreal.find_object(None, f"/Engine/Transient.ToolMenus_0:ToolMenu_{i}" )
            
            if not obj:
                continue
        
        menu_name = str(obj.menu_name)
        if menu_name == "None":
            continue
    
        menu_list.add(menu_name)

    return list(menu_list)

def print_menu_list(menu_list):
    menu_list = sorted(menu_list)
    title = "Tool Menus (%d)" % len(menu_list)
    index_width = len(str(len(menu_list)))
    width = max([len(title)] + [len(name) + index_width + 2 for name in menu_list])

    print("+" + "=" * (width + 2) + "+")
    print("| " + title.center(width) + " |")
    print("+" + "=" * (width + 2) + "+")

    for i, name in enumerate(menu_list, start=1):
        line = "%s. %s" % (str(i).rjust(index_width), name)
        print("| " + line.ljust(width) + " |")

    print("+" + "-" * (width + 2) + "+")

print_menu_list(list_menu())