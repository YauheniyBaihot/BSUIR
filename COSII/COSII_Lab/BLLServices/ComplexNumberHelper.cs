using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BLLEntities;

namespace BLLServices
{
    internal static class ComplexNumberHelper
    {
        #region Fields

        private static int _SumCount = 0;
        private static int _MulCount = 0;

        #endregion

        #region Properties

        public static int SumCount
        {
            get
            {
                return _SumCount;
            }
        }

        public static int MulCount
        {
            get
            {
                return _MulCount;
            }
        }

        #endregion

        #region Methods

        internal static ComplexNumber Sum(ComplexNumber ComplexNumber1, ComplexNumber ComplexNumber2)
        {
            _SumCount++;
            return new ComplexNumber(ComplexNumber1.Re + ComplexNumber2.Re, ComplexNumber1.Im + ComplexNumber2.Im);
        }

        internal static ComplexNumber Min(ComplexNumber ComplexNumber1, ComplexNumber ComplexNumber2)
        {
            _SumCount++;
            return new ComplexNumber(ComplexNumber1.Re - ComplexNumber2.Re, ComplexNumber1.Im - ComplexNumber2.Im);
        }

        internal static ComplexNumber Mul(ComplexNumber ComplexNumber1, ComplexNumber ComplexNumber2)
        {
            _MulCount++;
            return new ComplexNumber(ComplexNumber1.Re * ComplexNumber2.Re - ComplexNumber1.Im * ComplexNumber2.Im, ComplexNumber1.Re * ComplexNumber2.Im + ComplexNumber1.Im * ComplexNumber2.Re);
        }

        internal static ComplexNumber Pow(ComplexNumber ComplexNumber, int Pow)
        {
            ComplexNumber Result = new ComplexNumber(1);
            for (int i = 0; i < Pow; i++)
            {
                Result = new ComplexNumber(Result.Re * ComplexNumber.Re - Result.Im * ComplexNumber.Im, Result.Re * ComplexNumber.Im + Result.Im * ComplexNumber.Re);
            }
            return Result;
        }

        public static void Split(List<ComplexNumber> Array, List<ComplexNumber> EvenArray, List<ComplexNumber> OddArray)
        {
            for (int i = 0; i < Array.Count; i++)
            {
                if (i % 2 == 0)
                {
                    EvenArray.Add(Array[i]);
                }
                else
                {
                    OddArray.Add(Array[i]);
                }
            }
        }

        public static void ClearCounters()
        {
            _SumCount = 0;
            _MulCount = 0;
        }

        public static ComplexNumber GetMainRoot(int n)
        {
            double Phi = 2 * Math.PI / n;
            return new ComplexNumber(Math.Cos(Phi), Math.Sin(Phi));
        }

        public static ComplexNumber GetMainRootNeg(int n)
        {
            double Phi = 2 * Math.PI / n;
            return new ComplexNumber(Math.Cos(Phi), -Math.Sin(Phi));
        }

        #endregion
    }
}
