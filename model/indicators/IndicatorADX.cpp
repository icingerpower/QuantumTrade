#include "IndicatorADX.h"
#include <cmath>
#include <limits>

RECORD_INDICATOR(IndicatorADX);

QString IndicatorADX::id() const
{
    return "IndicatorADX";
}

QString IndicatorADX::name() const {
    return QObject::tr("ADX", "Average Directional Index");
}

QString IndicatorADX::description() const {
    return QObject::tr("Average Directional Index");
}

double IndicatorADX::compute(
    std::deque<std::vector<double>>& q,
    int colIndexLow,
    int colIndexHigh,
    int /*colIndexOpen*/,
    int colIndexClose,
    int /*colIndexVolume*/,
    const Tick*,
    const QMap<QString, QVariant>& params) const
{
    int period     = params.value(PAR_ID_SIZE_SAMPLE).toInt();
    int sizeSample = qMin(int(q.size()), period + 1);
    if (sizeSample < period + 1)
        return std::numeric_limits<double>::quiet_NaN();

    // 1) Compute True Range, +DM, -DM for each bar
    std::vector<double> tr(sizeSample - 1),
                        pDM(sizeSample - 1),
                        mDM(sizeSample - 1);

    for (int i = 1; i < sizeSample; ++i)
    {
        double H     = q[i][colIndexHigh];
        double Hprev = q[i - 1][colIndexHigh];
        double L     = q[i][colIndexLow];
        double Lprev = q[i - 1][colIndexLow];
        double Cprev = q[i - 1][colIndexClose];

        tr[i - 1] = std::max({
            H - L,
            std::fabs(H - Cprev),
            std::fabs(L - Cprev)
        });

        double upMove   = H     - Hprev;
        double downMove = Lprev - L;
        pDM[i - 1] = (upMove   > downMove && upMove   > 0) ? upMove   : 0.0;
        mDM[i - 1] = (downMove > upMove   && downMove > 0) ? downMove : 0.0;
    }

    // 2) Wilder smoothing of TR, +DM, -DM
    auto wilder = [&](const std::vector<double>& src)
    {
        std::vector<double> out(src.size());
        // first value = sum of first 'period' elements
        double sum = 0;
        for (int i = 0; i < period; ++i)
            sum += src[i];
        out[period - 1] = sum;
        // then recursive smoothing
        for (size_t i = period; i < src.size(); ++i)
            out[i] = out[i - 1] - (out[i - 1] / period) + src[i];
        return out;
    };

    auto sTR  = wilder(tr);
    auto sPDM = wilder(pDM);
    auto sMDM = wilder(mDM);

    // 3) Compute DX series
    std::vector<double> DX(sTR.size(), 0.0);
    for (size_t i = period - 1; i < sTR.size(); ++i)
    {
        double plusDI  = 100.0 * (sPDM[i] / sTR[i]);
        double minusDI = 100.0 * (sMDM[i] / sTR[i]);
        double diff    = std::fabs(plusDI - minusDI);
        double sumDI   = plusDI + minusDI;
        DX[i] = (sumDI > 0) ? (100.0 * (diff / sumDI)) : 0.0;
    }

    // 4) Smooth DX into ADX via Wilder smoothing
    auto adxSeries = wilder(DX);
    // Normalize by 'period' to get the average
    return adxSeries.back() / period;
}


QList<QMap<QString, QVariant>> IndicatorADX::possibleParams() const
{
    return {
        {{ PAR_ID_SIZE_SAMPLE, 14 }},  // classic 14-period ADX
        {{ PAR_ID_SIZE_SAMPLE, 7  }},  // shorter
        {{ PAR_ID_SIZE_SAMPLE, 28 }}   // longer
    };
}
