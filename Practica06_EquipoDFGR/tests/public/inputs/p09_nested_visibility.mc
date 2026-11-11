int x;
{
    int y = x;
    {
        int z = y + x;
        print(z);
    }
    print(y);
}
print(x);
