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