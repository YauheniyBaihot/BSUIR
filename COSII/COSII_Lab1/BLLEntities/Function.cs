using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BLLServices
{
    public class Function
    {
        private List<double> _X;
        private List<double> _Y;

        public List<double> X
        {
            get
            {
                return this._X;
            }
        }

        public List<double> Y
        {
            get
            {
                return this._Y;
            }
        }

        public Function(IEnumerable<double> X, IEnumerable<double> Y)
        {
            this._X = new List<double>(X);
            this._Y = new List<double>(Y);
        }        
    }
}
