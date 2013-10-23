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
            
        }
        
        public ActionResult Lab1()
        {
            return View(new Counter(FourierResults.DFTCount, FourierResults.FFTCount));
        }
        
        public ActionResult Lab2()
        {
            
        }
        
        public JsonResult GetOriginalFunction()
        {
            return Json(FourierResults.OriginalFunction);
        }

        public JsonResult GetFFT()
        {
            return Json(FourierResults.FFTFunction);
        }

        public JsonResult GetDFT()
        {
            return Json(FourierResults.DFTFunction);
        }

        public JsonResult GetPhaseFFT()
        {
            return Json(FourierResults.PhaseFFT);
        }

        public JsonResult GetAmplitudeFFT()
        {
            return Json(FourierResults.AmplitudeFFT);
        }

        public JsonResult GetPhaseDFT()
        {
            return Json(FourierResults.PhaseDFT);
        }

        public JsonResult GetAmplitudeDFT()
        {
            return Json(FourierResults.AmplitudeDFT);
        }
    }
}
