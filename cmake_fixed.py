#!/usr/bin/python

#Fixed zephyr cmake generate error with no userspace

import os

def add_patch(path):
  with open(path, "r+", encoding="utf-8") as fd:
    fbuffer = fd.read()
    if fbuffer.find("zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)") < 0:
      print("Writing:", path)
      fd.seek(0, 2)
      fd.write("\nzephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)")
    
def fixed_cmake_file(root_path, *k):
  path_list = []
  for dname in k:
    abs_path = os.path.join(root_path, "drivers/", dname, "CMakeLists.txt")
    add_patch(abs_path)

if __name__ == "__main__":
  fixed_cmake_file(os.path.join(os.getcwd(), 'zephyr_pkg/zephyr'), 
    'gpio', 'flash', 'spi', 'serial', 'kscan', 'pwm')
  
