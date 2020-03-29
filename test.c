int main()
{
    int i,a=1;
    for(i=0;i<10;++i){
        __builtin_print_u64(a*=(i+1));
    }
    return 0;
}
