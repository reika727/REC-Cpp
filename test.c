int main()
{
    int a=1,i;
    for(i=0;i<10;++i){
        __builtin_print_u64(a*=(i+1));
        __builtin_put_ascii(10);
    }
    return 0;
}
