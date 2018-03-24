filetype="au BufRead,BufNewFile *.br setf brook"
dir=`pwd`
syntaxfile="au! Syntax brook source $dir/brook.vim"

echo $filetype >> ~/.vimrc
echo $syntaxfile >> ~/.vimrc
