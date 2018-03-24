VIM SYNTAX FILE FOR BROOK
~~~~~~~~~~~~~~~~~~~~~~~~~
Since Brook is almost similar to cpp with some
additional keywords. This is a syntax file derived 
from the cpp syntax file for Vim.

FILES:
~~~~~
brook.vim:
    Vim syntax file for brook+ library

lnx_install.sh:
    Shell script for installing brook syntax file
    on linux/unix systems for the user who runs it.
    This uses brook.vim in the current directory so
    do not delete or move it.

win_install.bat:
    Batch file for installing brook syntax file on
    windows systems (all of the versions on which vim
    works). Assumes that you have VIM_EXE_DIR variable
    set. This is set by default if you enable "configure
    bat files for command line" option during Vim installation.

TROUBLESHOOTING:
~~~~~~~~~~~~~~~
If you are not able to get brook syntax highlighting to work
with vim using the install scripts, use the following steps:

1) copy "brook.vim" to "syntax" directory in vim install location.
2) add the following line to "filetype.vim" file in vim install dir:
    au BufRead,BufNewFile *.br setf brook
