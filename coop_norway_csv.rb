require 'date'

ColumnNames = %w(OfferId CustomerId ProductId ProductRelevance OfferRelevance BannerCode Region Store Sponsored StartDate EndDate)
start_date = DateTime.new(2020, 5, 1, 0, 0, 0)
end_date = start_date + 365 + 31 + 1 - Rational(1, 86_400)
OfferId          = [9000] * 15 + [9001] * 15
CustomerId       =    [format('%06d', 1)] * 15 +    [format('%06d', 2)] * 15
ProductId        = (10_000 .. 10_004).to_a * 6
ProductRelevance = (1..5).map { |x| x / 100.0 } * 6
OfferRelevance   = [0.25] * 30
BannerCode       = ([5000] * 5 + [5001] * 5 + [5002] * 5) * 2
Region           = [6000] * 5 + [6001] * 5 + [6000] * 20
Store            = [7000] * 5 + [7001] * 5 + [7000] * 20
Sponsored        = [0] * 30
StartDate        = [start_date.strftime('%Y/%m/%d %H:%M:%S')] * 30
EndDate          = [end_date.strftime('%Y/%m/%d %H:%M:%S')] * 30
arr = eval('[' + ColumnNames.join(', ') + ']').transpose
puts(arr.map { |x| x.join(',') })
