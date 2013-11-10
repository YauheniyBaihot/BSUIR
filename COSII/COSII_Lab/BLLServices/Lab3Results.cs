using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BLLEntities;

namespace BLLServices
{
    public static class Lab3Results
    {
        #region Fields

        private const int N = 8;
        private const double Period = 2 * Math.PI;
        private static Function _OriginalFunction;
        private static Function _WalshTransformFunction;
        private static Function _BackWalshTransformFunction;

        #endregion

        #region Properties

        public static Function OriginalFunction
        {
            get
            {
                return _OriginalFunction;
            }
        }

        public static Function WalshTransformFunction
        {
            get
            {
                return _WalshTransformFunction;
            }
        }

        public static Function BackWalshTransformFunction
        {
            get
            {
                return _BackWalshTransformFunction;
            }
        }

        #endregion

        #region Methods

        static Lab3Results()
        {
            List<double> OriginalY = new List<double>();
            List<Double> ArgumentsX = new List<Double>();
            for (int i = 0; i < N; i++)
            {
                double argument = i * (Period / N);
                ArgumentsX.Add(argument);
                OriginalY.Add(Math.Sin(2 * argument) + Math.Cos(3 * argument));
            }
            _OriginalFunction = new Function(ArgumentsX, OriginalY);

            var funcTrancformed = WalshTransform.FastWalshTransform(OriginalY.ToArray());
            _WalshTransformFunction = new Function(ArgumentsX, funcTrancformed.Select(y => y / N).ToList());

            var backTrancform = WalshTransform.FastWalshTransform(funcTrancformed.ToArray());
            _BackWalshTransformFunction = new Function(ArgumentsX, backTrancform.Select(y => y / N).ToList());
        }

        #endregion
    }
}
