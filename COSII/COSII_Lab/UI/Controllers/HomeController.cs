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
        public ActionResult Index()
        {
            return View();
        }       

        #region Lab1

        public ActionResult Lab1()
        {
            return View(new Counter(Lab1Results.DFTCount, Lab1Results.FFTCount));
        }

        public JsonResult GetOriginalFunction()
        {
            return Json(Lab1Results.OriginalFunction);
        }

        public JsonResult GetFFT()
        {
            return Json(Lab1Results.FFTFunction);
        }

        public JsonResult GetDFT()
        {
            return Json(Lab1Results.DFTFunction);
        }

        public JsonResult GetPhaseFFT()
        {
            return Json(Lab1Results.PhaseFFT);
        }

        public JsonResult GetAmplitudeFFT()
        {
            return Json(Lab1Results.AmplitudeFFT);
        }

        public JsonResult GetPhaseDFT()
        {
            return Json(Lab1Results.PhaseDFT);
        }

        public JsonResult GetAmplitudeDFT()
        {
            return Json(Lab1Results.AmplitudeDFT);
        }

        #endregion

        #region Lab2

        public ActionResult Lab2()
        {
            return View();
        }

        public JsonResult GetSinFunction()
        {
            return Json(Lab2Results.SinFunction);
        }

        public JsonResult GetCosFunction()
        {
            return Json(Lab2Results.CosFunction);
        }

        public JsonResult GetConvolutionFunction()
        {
            return Json(Lab2Results.ConvolutionFunction);
        }

        public JsonResult GetCorrelationFunction()
        {
            return Json(Lab2Results.CorrelationFunction);
        }

        public JsonResult GetFFTConvolutionFunction()
        {
            return Json(Lab2Results.FFTConvolutionFunction);
        }

        public JsonResult GetFFTCorrelationFunction()
        {
            return Json(Lab2Results.FFTCorrelationFunction);
        }

        #endregion

        #region Lab3

        public ActionResult Lab3()
        {
            return View();
        }

        public JsonResult GetWalshOriginalFunction()
        {
            return Json(Lab3Results.OriginalFunction);
        }

        public JsonResult GetWalshTransformFunction()
        {
            return Json(Lab3Results.WalshTransformFunction);
        }

        public JsonResult GetBackWalshTransformFunction()
        {
            return Json(Lab3Results.BackWalshTransformFunction);
        }

        #endregion

        #region Lab4

        public ActionResult Lab4()
        {
            return View();
        }

        #endregion
    }
}
