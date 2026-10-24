//Shadowing
int val = 100;
{
    int val = 50;
    print(val);
}
print(val);