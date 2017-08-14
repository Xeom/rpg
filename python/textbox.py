from so      import *
from sdl     import *
from border  import *
from display import *

class textbox_s(Structure):
    _fields_ = [
        ("bord",      border_p),
        ("font",      c_void_p),
        ("surf",      c_void_p),
        ("bordrrndr", c_void_p),
        ("area",      sdl_rect_s),
        ("textarea",  sdl_rect_s),
        ("vislines",  c_size_t),
        ("scroll",    c_size_t),
        ("attrs",     c_int),
        ("lnspacing", c_int),
        ("numlines",  c_size_t),
        ("text",      POINTER(c_char_p)),
    ]

textbox_p = POINTER(textbox_s)

so.textbox_init.argtypes = [textbox_p, border_p, c_void_p, c_void_p, c_int]
so.textbox_init.restype  = c_int

so.textbox_resize.argtypes = [c_void_p, c_void_p]
so.textbox_resize.restype  = None

so.textbox_draw.argtypes = [c_void_p]
so.textbox_draw.restype  = None

so.textbox_add_line.argtypes = [c_void_p, c_size_t]
so.textbox_add_line.restype  = None

so.textbox_set_line.argtypes = [c_void_p, c_size_t, c_char_p]
so.textbox_set_line.restype  = None

class Textbox:
    def __init__(self, ptr=None):
        if ptr == None:
            self.struct = textbox_s();
            self.ptr    = textbox_p(self.struct)
            self.ptr = cast(so.malloc(sizeof(textbox_s)), textbox_p)
            self.tofree = True

        else:
            self.tofree = False
            self.struct = ptr.contents
            self.ptr = ptr

    def __del__(self):
        pass
#        if self.tofree:
 #           so.free(self.ptr)

    def init(self, border, font, surf, attrs):
        so.textbox_init(self.ptr, border, font, surf, attrs)

    def resize(self, x, y, w, h):
        rect = sdl_rect_s(x, y, w, h)
        so.textbox_resize(self.ptr, byref(rect))

    def draw(self):
        so.textbox_draw(self.ptr)

    def add_line(self, i=None):
        so.textbox_add_line(self.ptr, i)

    def del_line(self, i):
        so.textbox_add_line(self.ptr, i)

    def set_line(self, i, text):
        print(i, text)
        so.textbox_set_line(self.ptr, i, text.encode("utf-8"))

class Menu:
    border_default = "wooden"
    font_default   = ("slkscr", 16)

    def __init__(self, surf, attrs, border=None, font=None):
        bordername = self.border_default if border == None else border
        fontname   = self.font_default   if font   == None else font

        self.border = Border.get(bordername)
        self.font   = Font.get(*fontname)

        self.textbox  = Textbox()

        self.textbox.init(self.border, self.font, surf, attrs)
        self.options  = []
        self.selected = None

        self.undrawn  = True

    def add_option(self, text):
        self.textbox.add_line(len(self.options))
        self.options.append(text)

        self.undrawn = True

        self.update_line(len(self.options) - 1)

        return len(self.options) - 1

    def update_line(self, i):
        if i == self.selected:
            self.textbox.set_line(i, "\\c4a3e0b \\cdf9a1b" + self.options[i])

        else:
            self.textbox.set_line(i, "\\c4a3e0b" + self.options[i])

        self.undrawn = True

    def select_option(self, i):
        prev = self.selected

        self.selected = i % len(self.options)

        if prev != None:
            self.update_line(prev)


        self.update_line(self.selected)

    def draw(self):
        self.textbox.draw()

    def on_option_choose(self, option):
        pass

    def on_option_select(self, option):
        pass

so.display_init()

m = Menu(cast(so.display_surface, POINTER(c_void_p)).contents, 15+128)
m.add_option("One")
m.add_option("Two")
m.add_option("Three")
m.add_option("Three")

m.add_option("Three")
m.add_option("Three")
m.add_option("Three")
m.add_option("Three")

def scroll(key):
    if key == "Up":
        if m.selected == None:
            m.select_option(1)
        m.select_option(m.selected - 1)

    if key == "Down":
        if m.selected == None:
            m.select_option(-1)
        m.select_option(m.selected + 1)

on_keydown.append(scroll)

import time

while cast(so.display_alive, POINTER(c_int)).contents.value:
    so.display_poll()
    m.draw()
    time.sleep(0.01)

#f = so.TTF_OpenFont(b"/home/xeom/dev/rpg/asset/font/LiberationMono-Bold.ttf", 12)
#b = border_s()
#b2 = border_s()
#
#so.border_init(byref(b), b"/home/xeom/dev/rpg/asset/art/borders/wooden.bmp")
#so.border_init(byref(b2), b"/home/xeom/dev/rpg/asset/art/borders/stone_brick.bmp")
#t = Textbox()
#t2 = Textbox()
#t2.init(b2, f, cast(so.display_surface, POINTER(c_void_p)).contents, 15)
#t.init(b, f, cast(so.display_surface, POINTER(c_void_p)).contents, 15)
#t.resize(16, 16, 128, 128)
#t.add_line(0)
#t.add_line(1)
#t.add_line(2)
#t.add_line(3)
#t.add_line(4)
#t.set_line(0, b"\c4a3e0bHello")
#t.set_line(1, b"\c4a3e0bHello")
#t.set_line(2, b"\cdf9a1b- World -")
#t.set_line(3, b"\c4a3e0bHello")
#t.set_line(4, b"\cd5c57bx Howdy x")
#t2.draw()
#t.draw()
#so.display_poll()
#import time
#time.sleep(10)
