
# Variables for configuring makefile
#  Seperated into seperate file not only for modularity, 
#  but so bash scripts etc. can read it,

VERSION=0.0.1

CC=gcc
LD=ar
AR=ar
LN=ln

SRC=src
INC=include
OBJ=obj

STD=c99
FOPTIONS=no-diagnostics-show-caret \
         diagnostics-color=always

WARNINGS=all no-unused-parameter extra pedantic \
         missing-prototypes fatal-errors format \
         conversion declaration-after-statement \
         error

LINKS=SDL2

INCLUDES=include/ src/

SRC_NAMES=terrain

DEPS_FILE=deps.mk

