using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BLLEntities;
using BLLServices;

namespace UITest
{
    class Program
    {
        static void Main(string[] args)
        {
            List<ComplexNumber> lst = new List<ComplexNumber>();
            List<Double> argumentsLst = new List<Double>();

            for (int i = 0; i < 16; i++)
            {
                double argument = i * (2 * Math.PI / 16);
                argumentsLst.Add(argument);
                lst.Add(new ComplexNumber(Math.Sin(3 * argument) + Math.Cos(argument)));
            }
            ComplexNumberHelper.ClearCounters();
            var preliminaryResult = FourierTransform.FFTDecimationInFrequency(lst, false);
            var FFTResult = FourierTransform.NormalizeAfterFFTDecimationInFrequency(preliminaryResult);
            for (int i = 0; i < 16; i++)
            {
                Console.WriteLine(preliminaryResult[i].Re);
                Console.WriteLine(FFTResult[i].Re);
                Console.WriteLine(preliminaryResult[i].Im);
                Console.WriteLine(FFTResult[i].Im);
            }
        }
    }
}
