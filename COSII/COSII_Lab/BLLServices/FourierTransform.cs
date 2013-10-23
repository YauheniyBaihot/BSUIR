using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BLLEntities;

namespace BLLServices
{
    internal static class FourierTransform
    {
        public static List<ComplexNumber> NormalizeAfterFFTDecimationInFrequency(List<ComplexNumber> Array)
        {            
            var y = new List<ComplexNumber>();
            if (Array.Count == 2)
            {
                return Array;
            }
            List<ComplexNumber> EvenPart = new List<ComplexNumber>();
            List<ComplexNumber> OddPart = new List<ComplexNumber>();
            ComplexNumberHelper.Split(Array, EvenPart, OddPart);
            var YEven = NormalizeAfterFFTDecimationInFrequency(EvenPart);
            var YOdd = NormalizeAfterFFTDecimationInFrequency(OddPart);
            return YEven.Union(YOdd).ToList();
        }

        public static List<ComplexNumber> FFTDecimationInFrequency(List<ComplexNumber> Array, bool IsBackTransform)
        {
            int n = Array.Count;
            if (n == 1)
            {
                return Array;
            }
            List<ComplexNumber> y = new List<ComplexNumber>();
            List<ComplexNumber> LowPart = new List<ComplexNumber>();
            List<ComplexNumber> OlderPart = new List<ComplexNumber>();
            var w = new ComplexNumber(1);
            var wn = IsBackTransform ? ComplexNumberHelper.GetMainRoot(n) : ComplexNumberHelper.GetMainRootNeg(n);
            for (int i = 0; i < n / 2; i++)
            {
                LowPart.Add(ComplexNumberHelper.Sum(Array[i], Array[i + n / 2]));
                OlderPart.Add(ComplexNumberHelper.Mul(ComplexNumberHelper.Min(Array[i], Array[i + n / 2]), w));
                w = ComplexNumberHelper.Mul(w, wn);
            }
            y = FFTDecimationInFrequency(LowPart, IsBackTransform).Union(FFTDecimationInFrequency(OlderPart, IsBackTransform)).ToList();
            return y;
        }

        public static List<ComplexNumber> DFT(List<ComplexNumber> array, List<double> initial, bool isBackTransform)
        {
            int n = array.Count;
            List<ComplexNumber> result = new List<ComplexNumber>();
            for (int i = 0; i < n; i++)
            {
                result.Add(new ComplexNumber());
                for (int j = 0; j < n; j++)
                {
                    var w = isBackTransform ? ComplexNumberHelper.GetMainRoot(n) : ComplexNumberHelper.GetMainRootNeg(n);
                    w = ComplexNumberHelper.Pow(w, j * i);
                    result[i] = ComplexNumberHelper.Sum(result[i], ComplexNumberHelper.Mul(w, array[j]));
                }
            }
            return result;
        }

        public static List<ComplexNumber> MutiplayArrays(List<ComplexNumber> Array1, List<ComplexNumber> Array2)
        {
            List<ComplexNumber> Result = new List<ComplexNumber>();
            for (int i = 0; i < Array1.Count; i++)
            {
                Result.Add(ComplexNumberHelper.Mul(Array1[i], Array2[i]));
            }
            return Result;
        }

        public static List<ComplexNumber> GetComplexConjugates(List<ComplexNumber> Array)
        {
            List<ComplexNumber> Result = new List<ComplexNumber>();
            foreach (var item in Array)
            {
                if (item.Im != 0)
                {
                    Result.Add(new ComplexNumber(item.Re, -item.Im));
                }
                else
                {
                    Result.Add(item);
                }
            }
            return Result;
        }
    }
}
