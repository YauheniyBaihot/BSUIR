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
        private const int N = 16;
        private const double Period = 2 * Math.PI;

        private static Function _OriginalFunction;
        private static Function _FFTFunction;
        private static Function _DFTFunction;
        private static Function _AmplitudeFFT;
        private static Function _PhaseFFT;
        private static Function _AmplitudeDFT;
        private static Function _PhaseDFT;
        private static List<double> ArgumentsX = new List<double>();

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

        static FourierResults()
        {
            List<ComplexNumber> lst = new List<ComplexNumber>();
            List<Double> argumentsLst = new List<Double>();
            for (int i = 0; i < N; i++)
            {
                ArgumentsX.Add((double)(i + 1));
                double argument = i * (Period / N);
                argumentsLst.Add(argument);
                lst.Add(new ComplexNumber(Math.Sin(3 * argument) + Math.Cos(argument)));
            }
            _OriginalFunction = new Function(argumentsLst, lst.Select(x => x.Re));

            ComplexNumberHelper.ClearCounters();
            var preliminaryResult = FourierTransform.FFTDecimationInFrequency(lst, false);
            var FFTResult = FourierTransform.NormalizeAfterFFTDecimationInFrequency(preliminaryResult);

            _PhaseFFT = new Function(ArgumentsX, FFTResult.Select(xx => xx.Phase));
            _AmplitudeFFT = new Function(ArgumentsX, FFTResult.Select(xxx => xxx.Amplitude));

            preliminaryResult = FourierTransform.FFTDecimationInFrequency(FFTResult, true);
            FFTResult = FourierTransform.NormalizeAfterFFTDecimationInFrequency(preliminaryResult);
            _FFTFunction = new Function(argumentsLst, FFTResult.Select(xa => xa.Re / N));

            ComplexNumberHelper.ClearCounters();
            var DFTResult = FourierTransform.DFT(lst, argumentsLst, false);

            _PhaseDFT = new Function(ArgumentsX, DFTResult.Select(xaa => xaa.Phase));
            _AmplitudeDFT = new Function(ArgumentsX, DFTResult.Select(xaa => xaa.Amplitude));

            DFTResult = FourierTransform.DFT(DFTResult, argumentsLst, true);
            _DFTFunction = new Function(argumentsLst, DFTResult.Select(xc => xc.Re / DFTResult.Count));
        }
    }
}
