using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BLLEntities;

namespace BLLServices
{
    public static class FourierTransform
    {
        public static List<ComplexNumber> NormalizeAfterFFTDecimationInFrequency(List<ComplexNumber> array)
        {
            var y = new List<ComplexNumber>();

            if (array.Count == 2)
            {
                return array;
            }

            List<ComplexNumber> evenPart = new List<ComplexNumber>();
            List<ComplexNumber> oddPart = new List<ComplexNumber>();
            ComplexNumberHelper.Split(array, evenPart, oddPart);

            var yEven = NormalizeAfterFFTDecimationInFrequency(evenPart);
            var yOdd = NormalizeAfterFFTDecimationInFrequency(oddPart);
            return yEven.Union(yOdd).ToList();
        }

        public static List<ComplexNumber> FFTDecimationInFrequency(List<ComplexNumber> array, bool isBackTransform)
        {
            int n = array.Count;

            if (n == 1)
            {
                return array;
            }

            List<ComplexNumber> y = new List<ComplexNumber>();
            List<ComplexNumber> lowPart = new List<ComplexNumber>();
            List<ComplexNumber> olderPart = new List<ComplexNumber>();

            var w = new ComplexNumber(1);
            var wn = isBackTransform ? ComplexNumberHelper.GetMainRoot(n) : ComplexNumberHelper.GetMainRootNeg(n);

            for (int i = 0; i < n / 2; i++)
            {
                lowPart.Add(ComplexNumberHelper.Sum(array[i], array[i + n / 2]));
                olderPart.Add(ComplexNumberHelper.Mul(ComplexNumberHelper.Min(array[i], array[i + n / 2]), w));
                w = ComplexNumberHelper.Mul(w, wn);
            }

            y = FFTDecimationInFrequency(lowPart, isBackTransform).Union(FFTDecimationInFrequency(olderPart, isBackTransform)).ToList();

            return y;
        }

        public static List<ComplexNumber> DFT(List<ComplexNumber> array, List<double> initial, bool isBackTransform)
        {
            var n = array.Count;
            List<ComplexNumber> result = new List<ComplexNumber>();

            for (int i = 0; i < n; i++)
            {
                result[i] = new ComplexNumber();
                for (int m = 0; m < n; m++)
                {
                    var w = isBackTransform ? ComplexNumberHelper.GetMainRoot(n) : ComplexNumberHelper.GetMainRootNeg(n);
                    w = ComplexNumberHelper.Pow(w, m * i);
                    result[i] = ComplexNumberHelper.Sum(result[i], ComplexNumberHelper.Mul(w, array[m]));
                }
            }

            return result;
        }
    }
}
