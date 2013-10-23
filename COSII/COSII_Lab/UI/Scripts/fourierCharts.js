﻿function drawCharts(title, type, chartId) {
    $(chartId).dxChart({
        dataSource: dataSource,
        commonSeriesSettings: {
            type: type,
            argumentField: "x"
        },
        commonAxisSettings: {
            grid: {
                visible: true
            }
        },
        series: series,
        tooltip: {
            enabled: true
        },
        legend: {
            verticalAlignment: "bottom",
            horizontalAlignment: "center"
        },
        title: title,
        commonPaneSettings: {
            border: {
                visible: true,
                bottom: false
            }
        }
    });
}

function pushOriginalFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], original: data.Y[i] });
    }
    series.push({ valueField: 'original', name: 'sin(3x) + cos(3x)' });
}

function pushFFT(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], FFT: data.Y[i] });
    }
    series.push({ valueField: 'FFT', name: 'FFT' });
}

function pushFFTPhase(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], FFTPhase: data.Y[i] });
    }
    series.push({ valueField: 'FFTPhase', name: 'Phase Spectrum FFT' });
}

function pushFFTAmplitude(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], FFTAmplitude: data.Y[i] });
    }
    series.push({ valueField: 'FFTAmplitude', name: 'Amlitude Spectrum FFT' });
}

function pushDFT(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], DFT: data.Y[i] });
    }
    series.push({ valueField: 'DFT', name: 'DFT' });
}

function pushDFTPhase(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], DFTPhase: data.Y[i] });
    }
    series.push({ valueField: 'DFTPhase', name: 'Phase Spectrum DFT' });
}

function pushDFTAmplitude(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], DFTAmplitude: data.Y[i] });
    }
    series.push({ valueField: 'DFTAmplitude', name: 'Amlitude Spectrum DFT' });
}

function pushSinFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], sin: data.Y[i] });
    }
    series.push({ valueField: 'sin', name: 'sin(5x)' });
}

function pushCosFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], cos: data.Y[i] });
    }
    series.push({ valueField: 'cos', name: 'cos(2x)' });
}

function pushConvolutionFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], Convolution: data.Y[i] });
    }
    series.push({ valueField: 'Convolution', name: 'convolution' });
}

function pushCorrelationFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], Correlation: data.Y[i] });
    }
    series.push({ valueField: 'Correlation', name: 'correlation' });
}

function getFunction(callback, viewName, chartId, type, title) {
    $.ajax({
        url: "/Home/" + viewName,
        type: "POST",
        dataType: "json",
        success: function (data) {
            callback(data);
            drawCharts(title, type, chartId);
        }
    });
}

function clearData() {
    dataSource = [];
    series = [];
}