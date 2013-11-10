function drawCharts(title, type, chartId) {
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