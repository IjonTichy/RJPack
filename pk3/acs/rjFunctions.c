function int unusedTID(int start, int end)
{
    int ret = start - 1;
    int tidNum;

    if (start > end)
    {
        return -127;
    }
    
    while (ret++ != end)
    {
        if (ThingCount(0, ret) == 0)
        {
            return ret;
        }
    }
    
    return -1;
}

function int abs (int x)
{
    if (x < 0) {return -x;}
    return x;
}


function int min(int x, int y)
{
    if (x < y) {return x;}
    return y;
}

function int max(int x, int y)
{
    if (x > y) {return x;}
    return y;
}

function int middle(int x, int y, int z)
{
    if ((x < z) && (y < z))
    {
        return min(max(x, y), z);
    }

    return max(min(x, y), z);
}

function int round(int x)
{
    int frac = abs(x % 1.0);
    int intg = (x >> 16);

    if (frac < 0.5)
    {
        return intg;
    }

    return intg + 1;
}

function int modulo(int x, int y)
{
    return x - ((x/y) * y);
}
