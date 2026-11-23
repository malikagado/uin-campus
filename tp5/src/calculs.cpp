double somme(std::span<const double> valeurs)
{
return std::accumulate(valeurs.begin(), valeurs.end(), 0.0);
}
double moyenne(std::span<const double> notes)
{
assert(!notes.empty() && "moyenne() : aucune note");
return somme(notes) / static_cast<double>(notes.size());
}
// moyenne pondérée, calculerStatistiques, compterValidees :
// remplacer const std::vector<double>& par std::span<const double>, corps inchangés
