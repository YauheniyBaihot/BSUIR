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
    series.push({ valueField: 'Convolution', name: 'Convolution' });
}

function pushCorrelationFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], Correlation: data.Y[i] });
    }
    series.push({ valueField: 'Correlation', name: 'Correlation' });
}

function pushFFTConvolutionFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], FFTConvolution: data.Y[i] });
    }
    series.push({ valueField: 'FFTConvolution', name: 'FFT Convolution' });
}

function pushFFTCorrelationFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], FFTCorrelation: data.Y[i] });
    }
    series.push({ valueField: 'FFTCorrelation', name: 'FFT Correlation' });
}