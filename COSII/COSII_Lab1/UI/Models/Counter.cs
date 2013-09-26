using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace UI.Models
{
    public class Counter
    {
        public int DFTCount;
        public int FFTCount;

        public Counter()
        {

        }

        public Counter(int DFTCount, int FFTCount)
        {
            this.DFTCount = DFTCount;
            this.FFTCount = FFTCount;
        }
    }
}