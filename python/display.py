from so import *

so.display_init.argtypes = []
so.display_init.restype  = None

so.display_kill.argtypes = []
so.display_kill.restype  = None

so.display_poll.argtypes = []
so.display_poll.restype  = None

display_height_ptr = cast(so.display_height, POINTER(c_int))
display_width_ptr  = cast(so.display_width,  POINTER(c_int))

def display_get_height():
    return height_ptr.contents.value

def display_get_width():
    return width_ptr.contents.value

on_keydown = []

def run_on_keydown(keyname):
    for f in on_keydown:
        f(keyname.decode("ascii"))

keyfunct = CFUNCTYPE(None, c_char_p)

run_on_keydown_c = cast(keyfunct(run_on_keydown), c_void_p).value
cast(so.display_on_keydown, POINTER(c_void_p)).contents.value = run_on_keydown_c
