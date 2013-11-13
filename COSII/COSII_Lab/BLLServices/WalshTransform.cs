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
    }
}
