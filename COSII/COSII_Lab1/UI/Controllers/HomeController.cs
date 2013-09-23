using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using BLLEntities;
using BLLServices;
using UI.Models;

namespace UI.Controllers
{
    public class HomeController : Controller
    {
        private const int N = 16;
        private const double Period = 2 * Math.PI;

        public ActionResult Index()
        {
            return View();
        }

        public ActionResult Separately()
        {
            List<ComplexNumber> lst = new List<ComplexNumber>();
            List<Double> argumentsLst = new List<Double>();
            for (int i = 0; i < N; i++)
            {
                double argument = i * (Period / N);
                argumentsLst.Add(argument);
                lst.Add(new ComplexNumber(Math.Sin(3 * argument) + Math.Cos(argument)));
            }
            return View(new Chart(argumentsLst, lst.Select(x => x.Re)));
        }

        public ActionResult Comparison()
        {
            return View();
        }

        public JsonResult GetOriginalFunction()
        {
            List<ComplexNumber> lst = new List<ComplexNumber>();
            List<Double> argumentsLst = new List<Double>();
            for (int i = 0; i < N; i++)
            {
                double argument = i * (Period / N);
                argumentsLst.Add(argument);
                lst.Add(new ComplexNumber(Math.Sin(3 * argument) + Math.Cos(argument)));
            }
            return Json(new Chart(argumentsLst, lst.Select(x => x.Re)));
        }

        public JsonResult GetAfterFFT()
        {
            List<ComplexNumber> lst = new List<ComplexNumber>();
            List<Double> argumentsLst = new List<Double>();
            for (int i = 0; i < N; i++)
            {
                double argument = i * (Period / N);
                argumentsLst.Add(argument);
                lst.Add(new ComplexNumber(Math.Sin(3 * argument) + Math.Cos(argument)));
            }
            ComplexNumberHelper.ClearCounters();
            var preliminaryResult = FourierTransform.FFTDecimationInFrequency(lst, false);
            var FFTResult = FourierTransform.NormalizeAfterFFTDecimationInFrequency(preliminaryResult);
            return Json(new Chart(FFTResult.Select(x => x.Phase), FFTResult.Select(x => x.Amplitude)));
        }
    }
}
