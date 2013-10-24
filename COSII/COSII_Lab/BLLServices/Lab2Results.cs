using BLLEntities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BLLServices
{
    public static class Lab2Results
    {
        #region Fields

        private const int N = 32;
        private const double Period = 2 * Math.PI;
        private static Function _SinFunction;
        private static Function _CosFunction;
        private static Function _ConvolutionFunction;
        private static Function _CorrelationFunction;

        #endregion

        #region Properties

        public static Function SinFunction
        {
            get
            {
                return _SinFunction;
            }
        }

        public static Function CosFunction
        {
            get
            {
                return _CosFunction;
            }
        }

        public static Function ConvolutionFunction
        {
            get
            {
                return _ConvolutionFunction;
            }
        }

        public static Function CorrelationFunction
        {
            get
            {
                return _CorrelationFunction;
            }
        }

        #endregion

        #region Methods

        static Lab2Results()
        {
            List<ComplexNumber> Sin = new List<ComplexNumber>();
            List<ComplexNumber> Cos = new List<ComplexNumber>();
            List<Double> ArgumentsX = new List<Double>();
            for (int i = 0; i < N; i++)
            {
                double argument = i * (Period / N);
                ArgumentsX.Add(argument);
                Cos.Add(new ComplexNumber(Math.Cos(2 * argument)));
                Sin.Add(new ComplexNumber(Math.Sin(5 * argument)));
            }
            _SinFunction = new Function(ArgumentsX, Sin.Select(x => x.Re));
            _CosFunction = new Function(ArgumentsX, Cos.Select(x => x.Re));

            var FFTResult = FourierTransform.FFTDecimationInFrequency(Sin, false);
            var FFTResultSin = FourierTransform.NormalizeAfterFFTDecimationInFrequency(FFTResult);
            FFTResult = FourierTransform.FFTDecimationInFrequency(Cos, false);
            var FFTResultCos = FourierTransform.NormalizeAfterFFTDecimationInFrequency(FFTResult);

            var MultiplayFunctions = FourierTransform.MutiplayArrays(FFTResultSin, FFTResultCos);
            FFTResult = FourierTransform.FFTDecimationInFrequency(MultiplayFunctions, true);
            var resultConvolution = FourierTransform.NormalizeAfterFFTDecimationInFrequency(FFTResult);
            _ConvolutionFunction = new Function(ArgumentsX, resultConvolution.Select(x => x.Re));

            var FFTResultSinConjugates = FourierTransform.GetComplexConjugates(FFTResultSin);
            MultiplayFunctions = FourierTransform.MutiplayArrays(FFTResultSinConjugates, FFTResultCos);
            FFTResult = FourierTransform.FFTDecimationInFrequency(MultiplayFunctions, true);
            var resultCorrelation = FourierTransform.NormalizeAfterFFTDecimationInFrequency(FFTResult);
            _CorrelationFunction = new Function(ArgumentsX, resultCorrelation.Select(x => x.Re));
        }

        #endregion
    }
}
