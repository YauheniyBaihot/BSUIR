using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BLLEntities;

namespace BLLServices
{
    public static class FourierResults
    {
        private const int N = 8;
        private const double Period = 2 * Math.PI;

        private static Function _OriginalFunction;
        private static Function _FFTFunction;
        private static Function _DFTFunction;
        private static Function _AmplitudeFFT;
        private static Function _PhaseFFT;
        private static Function _AmplitudeDFT;
        private static Function _PhaseDFT;
        private static int _FFTCount = 0;
        private static int _DFTCount = 0;

        public static Function OriginalFunction
        {
            get
            {
                return _OriginalFunction;
            }
        }

        public static Function FFTFunction
        {
            get
            {
                return _FFTFunction;
            }
        }

        public static Function DFTFunction
        {
            get
            {
                return _DFTFunction;
            }
        }

        public static Function AmplitudeFFT
        {
            get
            {
                return _AmplitudeFFT;
            }
        }

        public static Function PhaseFFT
        {
            get
            {
                return _PhaseFFT;
            }
        }

        public static Function AmplitudeDFT
        {
            get
            {
                return _AmplitudeDFT;
            }
        }

        public static Function PhaseDFT
        {
            get
            {
                return _PhaseDFT;
            }
        }

        public static int FFTCount
        {
            get
            {
                return _FFTCount;
            }
        }

        public static int DFTCount
        {
            get
            {
                return _DFTCount;
            }
        }

        static FourierResults()
        {
            List<ComplexNumber> OriginalX = new List<ComplexNumber>();
            List<Double> ArgumentsX = new List<Double>();
            for (int i = 0; i < N; i++)
            {
                double argument = i * (Period / N);
                ArgumentsX.Add(argument);
                OriginalX.Add(new ComplexNumber(Math.Sin(3 * argument) + Math.Cos(3 * argument)));
            }
            _OriginalFunction = new Function(ArgumentsX, OriginalX.Select(x => x.Re));

            ComplexNumberHelper.ClearCounters();
            var FFTResult = FourierTransform.FFTDecimationInFrequency(OriginalX, false);
            FFTResult = FourierTransform.NormalizeAfterFFTDecimationInFrequency(FFTResult);

            _PhaseFFT = new Function(ArgumentsX, FFTResult.Select(x => x.Phase));
            _AmplitudeFFT = new Function(ArgumentsX, FFTResult.Select(x => x.Amplitude));

            FFTResult = FourierTransform.FFTDecimationInFrequency(FFTResult, true);
            FFTResult = FourierTransform.NormalizeAfterFFTDecimationInFrequency(FFTResult);
            _FFTFunction = new Function(ArgumentsX, FFTResult.Select(x => x.Re / N));

            _FFTCount = ComplexNumberHelper.MulCount;

            ComplexNumberHelper.ClearCounters();
            var DFTResult = FourierTransform.DFT(OriginalX, ArgumentsX, false);

            _PhaseDFT = new Function(ArgumentsX, DFTResult.Select(x => x.Phase));
            _AmplitudeDFT = new Function(ArgumentsX, DFTResult.Select(x => x.Amplitude));

            DFTResult = FourierTransform.DFT(DFTResult, ArgumentsX, true);
            _DFTFunction = new Function(ArgumentsX, DFTResult.Select(x => x.Re / N));

            _DFTCount = ComplexNumberHelper.MulCount;
        }
    }
}
