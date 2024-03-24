using System;

public class Blowfish {
    // Определение констант
    private const int N = 16;
    private const int P_SIZE = 18;

    // Массивы P и S для ключа
    private uint[] P = new uint[P_SIZE];
    private uint[,] S = new uint[N, 256];

    // Подключение ключа
    public void SetKey(byte[] key) {
        if (key.Length < 1 || key.Length > 56)
            throw new ArgumentException("Key size must be between 1 and 56 bytes.");

        // Инициализация P и S
        for (int i = 0; i < P_SIZE; i++)
            P[i] = P[i] ^ BitConverter.ToUInt32(key, (i * 4) % key.Length);

        uint[] zeroBlock = new uint[2];

        for (int i = 0; i < P_SIZE; i += 2) {
            EncryptBlock(zeroBlock);
            P[i] = zeroBlock[0];
            P[i + 1] = zeroBlock[1];
        }

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < 256; j += 2) {
                EncryptBlock(zeroBlock);
                S[i, j] = zeroBlock[0];
                S[i, j + 1] = zeroBlock[1];
            }
        }
    }

    // Шифрование блока
    public void EncryptBlock(uint[] block) {
        uint left = block[0];
        uint right = block[1];
        
        // 16 раундов шифрования (page1)
        for (int i = 0; i < N; i++) {
            left ^= P[i];
            right ^= F(left);
            Swap(ref left, ref right);
        }

        // 17 и 18 раунды
        Swap(ref left, ref right);
        right ^= P[N];
        left ^= P[N + 1];

        block[0] = left;
        block[1] = right;
    }

    // Дешифрование блока
    public void DecryptBlock(uint[] block) {
        uint left = block[0];
        uint right = block[1];

        for (int i = N + 1; i > 1; i--) {
            left ^= P[i];
            right ^= F(left);
            Swap(ref left, ref right);
        }

        Swap(ref left, ref right);
        right ^= P[1];
        left ^= P[0];

        block[0] = left;
        block[1] = right;
    }

    // Функция F
    private uint F(uint x) {
        byte a, b, c, d;
        d = (byte)(x & 0xFF);
        x >>= 8;
        c = (byte)(x & 0xFF);
        x >>= 8;
        b = (byte)(x & 0xFF);
        x >>= 8;
        a = (byte)(x & 0xFF);
        return (S[0, a] + S[1, b]) ^ S[2, c] + S[3, d];
    }

    // Обмен значений
    private void Swap(ref uint a, ref uint b) {
        (a, b) = (b, a);
    }
    
    public byte[] EncryptMessage(byte[] plaintext) {
        // Дополняем сообщение до кратного размера блока
        int paddedLength = ((plaintext.Length + 7) / 8) * 8;
        byte[] paddedPlaintext = new byte[paddedLength];
        Array.Copy(plaintext, paddedPlaintext, plaintext.Length);

        // Шифруем блоки данных
        for (int i = 0; i < paddedPlaintext.Length; i += 8) {
            uint[] block = new uint[2];
            block[0] = BitConverter.ToUInt32(paddedPlaintext, i);
            block[1] = BitConverter.ToUInt32(paddedPlaintext, i + 4);
            EncryptBlock(block);
            Array.Copy(BitConverter.GetBytes(block[0]), 0, paddedPlaintext, i, 4);
            Array.Copy(BitConverter.GetBytes(block[1]), 0, paddedPlaintext, i + 4, 4);
        }

        return paddedPlaintext;
    }

    // Функция для дешифрования сообщения
    public byte[] DecryptMessage(byte[] ciphertext) {
        // Дешифруем блоки данных
        for (int i = 0; i < ciphertext.Length; i += 8) {
            uint[] block = new uint[2];
            block[0] = BitConverter.ToUInt32(ciphertext, i);
            block[1] = BitConverter.ToUInt32(ciphertext, i + 4);
            DecryptBlock(block);
            Array.Copy(BitConverter.GetBytes(block[0]), 0, ciphertext, i, 4);
            Array.Copy(BitConverter.GetBytes(block[1]), 0, ciphertext, i + 4, 4);
        }

        // Удаляем дополнение
        int padLength = ciphertext[ciphertext.Length - 1];
        byte[] unpaddedText = new byte[ciphertext.Length - padLength];
        Array.Copy(ciphertext, unpaddedText, unpaddedText.Length);

        return unpaddedText;
    }
}