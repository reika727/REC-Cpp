int main()
{
    int i;
    for(i=1024;i>0;i/=2){
        __builtin_print_u64(i);
        __builtin_put_ascii(10);
    }
    return 0;
}
