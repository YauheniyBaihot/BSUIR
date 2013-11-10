function pushOriginalFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], original: data.Y[i] });
    }
    series.push({ valueField: 'original', name: 'sin(3x) + cos(3x)' });
}

function pushWalshTransformFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], WalshTransformFunction: data.Y[i] });
    }
    series.push({ valueField: 'WalshTransformFunction', name: 'Walsh Transform Function' });
}

function pushBackWalshTransformFunction(data) {
    clearData();
    for (var i = 0; i < data.X.length; i++) {
        dataSource.push({ x: data.X[i], BackWalshTransformFunction: data.Y[i] });
    }
    series.push({ valueField: 'BackWalshTransformFunction', name: 'Back Walsh Transform Function' });
}