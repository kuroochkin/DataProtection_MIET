using System.Text;

namespace LR2;

public class RSAEncryption
{
    private long _p, _q, _n, _phi, _e, _d;

    public RSAEncryption(long p, long q)
    {
        this._p = p;
        this._q = q;
        this._n = p * q;
        this._phi = (p - 1) * (q - 1);
        this._e = CalculateE();
        this._d = CalculateD();
    }

    private long CalculateE()
    {
        for (long i = 2; i < _phi; i++)
        {
            if (Gcd(i, _phi) == 1)
            {
                return i;
            }
        }
        return 1;
    }

    private long Gcd(long a, long b)
    {
        while (b != 0)
        {
            long temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    private long CalculateD()
    {
        long d = 0;
        while ((d * _e) % _phi != 1)
        {
            d++;
        }
        return d;
    }

    public string Encrypt(string message)
    {
        byte[] bytes = Encoding.UTF8.GetBytes(message);
        long[] encryptedBytes = new long[bytes.Length];
        for (int i = 0; i < bytes.Length; i++)
        {
            encryptedBytes[i] = ModPow(bytes[i], _e, _n);
        }
        return string.Join(",", encryptedBytes);
    }

    public string Decrypt(string encryptedMessage)
    {
        string[] encryptedBytesStr = encryptedMessage.Split(',');
        long[] encryptedBytes = Array.ConvertAll(encryptedBytesStr, long.Parse);
        byte[] decryptedBytes = new byte[encryptedBytes.Length];
        for (int i = 0; i < encryptedBytes.Length; i++)
        {
            decryptedBytes[i] = (byte)ModPow(encryptedBytes[i], _d, _n);
        }
        return Encoding.UTF8.GetString(decryptedBytes);
    }

    private long ModPow(long baseValue, long exponent, long modulus)
    {
        if (modulus == 1) return 0;
        long result = 1;
        baseValue %= modulus;
        while (exponent > 0)
        {
            if (exponent % 2 == 1)
                result = (result * baseValue) % modulus;
            exponent >>= 1;
            baseValue = (baseValue * baseValue) % modulus;
        }
        return result;
    }
}