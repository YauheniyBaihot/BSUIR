using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections;

namespace BLLServices
{
    public static class WalshTransform
    {
        public static double[] FastWalshTransform(double[] Array)
        {
            int N = Array.Length;
            if (N == 1)
            {
                return Array;
            }
            double[] LowPart = new double[N / 2];
            double[] ElderPart = new double[N / 2];
            double[] LowResult = new double[N / 2];
            double[] ElderResult = new double[N / 2];
            double[] x = new double[N];
            for (int i = 0; i < N / 2; i++)
            {
                LowPart[i] = Array[i] + Array[i + N / 2];
                ElderPart[i] = Array[i] - Array[i + N / 2];
            }
            LowResult = WalshTransform.FastWalshTransform(LowPart);
            ElderResult = WalshTransform.FastWalshTransform(ElderPart);
            int k = 0;
            for (int j = 0; j < N / 2; k++, j++)
            {
                x[k] = LowResult[j];
            }
            for (int j = 0; j < N / 2; k++, j++)
            {
                x[k] = ElderResult[j];
            }
            return x;
        }

        public static double OrderByHadamard(int Number, double T, int N)
        {
            byte[] Buffer = BitConverter.GetBytes(Number);
            BitArray Array = new BitArray(Buffer);
            Array = WalshTransform.Revers(Array, N);
            BitArray GrayCode = WalshTransform.GetBackGrayCode(Array);
            int NumberByHadamar = BitConverter.ToInt32(WalshTransform.BitArrayToByteArray(GrayCode), 0);
            return WalshTransform.WalshFunction(NumberByHadamar, T);
        }

        public static byte[] BitArrayToByteArray(BitArray Array)
        {
            byte[] Result = new byte[Array.Length / 8];
            Array.CopyTo(Result, 0);
            return Result;
        }

        public static BitArray GetBackGrayCode(BitArray GrayCode)
        {
            BitArray BinaryCode = new BitArray(GrayCode.Count);
            for (int i = BinaryCode.Count - 1; i >= 0; i--)
            {
                if (i != BinaryCode.Count - 1)
                {
                    BinaryCode[i] = GrayCode[i] == BinaryCode[i + 1] ? false : true;
                }
                else
                {
                    BinaryCode[i] = GrayCode[i];
                }
            }
            return BinaryCode;
        }

        public static double WalshFunction(int Number, double T)
        {
            if (Number == 1)
            {
                return 1;
            }
            BitArray GrayCode = WalshTransform.GetGrayCode(new BitArray(BitConverter.GetBytes(Number)));
            double WalshFunction = 1;
            for (int i = 0; i <= GetElderBit(GrayCode); i++)
            {
                if (GrayCode[i])
                {
                    WalshFunction = Math.Sin(Math.Pow(2, i + 1) * Math.PI * T) >= 0 ? 1 : -1;
                }
                else
                {
                    WalshFunction = 1;
                }
            }
            return WalshFunction;
        }

        public static BitArray GetGrayCode(BitArray BinaryCode)
        {
            BitArray GrayCode = new BitArray(BinaryCode.Count);
            for (int i = 0; i < BinaryCode.Count; i++)
            {
                if (i + 1 != BinaryCode.Count)
                {
                    GrayCode[i] = BinaryCode[i] == BinaryCode[i + 1] ? false : true;
                }
                else
                {
                    GrayCode[i] = BinaryCode[i];
                }
            }
            return GrayCode;
        }        

        private static int GetElderBit(BitArray Array)
        {
            int ElderBit = 0;
            for (int i = Array.Count - 1; i >= 0; i--)
            {
                if (Array[i])
                {
                    ElderBit = i;
                    break;
                }
            }
            return ElderBit;
        }

        public static BitArray Revers(BitArray Array, int N)
        {
            bool temp;
            int ElderBit = (int)Math.Log(N, 2);
            for (int i = 0; i <= ElderBit / 2; i++)
            {
                temp = Array[i];
                Array[i] = Array[ElderBit - i - 1];
                Array[ElderBit - i - 1] = temp;
            }
            return Array;
        }              
    }
}
