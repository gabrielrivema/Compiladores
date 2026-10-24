//Uso de variable global en ambitos anidados
int global_target = 99;
{
    {
        {
            global_target = global_target + 1;
            print(global_target);
        }
    }
}