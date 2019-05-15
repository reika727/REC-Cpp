function! RegisterHighlight()
    syntax match RegName "\"%\w\+\"" display containedin=ALL
    highlight link RegName Special
endf
autocmd BufNew,BufRead * call RegisterHighlight()
