set filetype=au BufRead,BufNewFile *.br setf brook

copy brook.vim "%VIM_EXE_DIR%\syntax"
echo %filetype% >> "%VIM_EXE_DIR%\..\_vimrc"
