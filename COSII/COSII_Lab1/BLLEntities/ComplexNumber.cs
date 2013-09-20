using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BLLEntities
{
    public class ComplexNumber
    {
        #region Fields;

        private double _Re;
        private double _Im;

        #endregion

        #region Properties

        public double Re 
        {
            get
            {
                return _Re;
            }
            set
            {
                _Re = value;
            }
        }

        public double Im
        {
            get
            {
                return _Im;
            }
            set
            {
                _Im = value;
            }
        }

        public double Amplitude
        {
            get
            {
                return Math.Sqrt(Math.Pow(_Re, 2) + Math.Pow(_Im, 2));
            }
        }

        public double Phase
        {
            get
            {
                return 1 / Math.Tan(_Im / _Re);
            }
        }

        #endregion

        #region Constructors

        public ComplexNumber()
        {
            _Re = 0;
            _Im = 0;
        }

        public ComplexNumber(double Re, double Im)
        {
            _Re = Re;
            _Im = Im;
        }

        #endregion
    }
}
