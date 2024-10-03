so $VIMRUNTIME/defaults.vim
set tabstop=4 shiftwidth=4 softtabstop=4
set nu

"insert new lines without moving cursor or entering insert mode
nnoremap <silent> <c-n> :<C-u>call append(line("."),   repeat([""], v:count1))<CR>
nnoremap <silent> <c-p> :<C-u>call append(line(".")-1, repeat([""], v:count1))<CR>

"insert line break without entering edit mode.
nnoremap <Enter> i<Enter><esc>k$
