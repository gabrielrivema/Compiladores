//Shadowing + nombres duplicados
int x = 10;
{
    bool x = true;
    {
        int x = 20;
        print(x);
    }
    print(x);
}
print(x);