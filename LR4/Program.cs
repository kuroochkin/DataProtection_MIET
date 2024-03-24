using System;
using System.Text;

namespace LR4
{
    class Program
    {
        static void Main(string[] args)
        {
            var blowfish = new Blowfish();

            // Устанавливаем ключ
            byte[] key = { 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF };
            blowfish.SetKey(key);

            // Шифруем сообщение
            string message = "Hello, world!";
            byte[] plaintext = Encoding.UTF8.GetBytes(message);
            byte[] ciphertext = blowfish.EncryptMessage(plaintext);

            // Дешифруем сообщение
            byte[] decryptedText = blowfish.DecryptMessage(ciphertext);
            string decryptedMessage = Encoding.UTF8.GetString(decryptedText);

            // Выводим результаты
            Console.WriteLine("Original message: " + message);
            Console.WriteLine("Encrypted message: " + BitConverter.ToString(ciphertext).Replace("-", ""));
            Console.WriteLine("Decrypted message: " + decryptedMessage);
        }
    }
}