using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace UI.Models
{
    public class Chart
    {
        public Chart(IEnumerable<double> X, IEnumerable<double> Y)
        {
            this.X = new List<double>(X);
            this.Y = new List<double>(Y);
        }

        public List<double> X;
        public List<double> Y;
    }
}