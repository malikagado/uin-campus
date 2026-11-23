#include "uincampus/etudiant.hpp"
#include <cassert>
#include <format>
#include <stdexcept>
#include <utility>
namespace uin {
Etudiant::Etudiant(std::string matricule, std::string nom, std::string prenom)
: matricule_{std::move(matricule)}, nom_{std::move(nom)}, prenom_{std::move(prenom)}
{
if (matricule_.empty()) {
throw std::invalid_argument("Étudiant : le matricule est obligatoire");
}
if (nom_.empty() || prenom_.empty()) {
throw std::invalid_argument(
std::format("Étudiant {} : le nom et le prénom sont obligatoires",
matricule_));
}
}
std::string Etudiant::nomComplet() const
{
return std::format("{} {}", prenom_, nom_);
}
void Etudiant::enregistrerResultat(const UE& ue, Note note)
{
for (ResultatUE& resultat : resultats_) {
if (resultat.ue.code() == ue.code()) {
resultat.note = note;
// l'invariant « un résultat par UE » est préservé
return;
}
}
resultats_.push_back(ResultatUE{ue, note});
}
double Etudiant::moyenne() const
{
assert(aDesResultats() && "Etudiant::moyenne() : aucun résultat");
double sommePonderee{0.0};
double sommeCoefficients{0.0};
for (const auto& [ue, note] : resultats_) {
sommePonderee += note.valeur() * ue.coefficient();
sommeCoefficients += ue.coefficient();
}
return sommePonderee / sommeCoefficients;
}
int Etudiant::creditsObtenus() const
{
int total{0};
for (const auto& [ue, note] : resultats_) {
if (note.estValidante()) {
total += ue.credits();
}
}
return total;
}
int Etudiant::creditsInscrits() const
{
int total{0};
for (const auto& resultat : resultats_) {
total += resultat.ue.credits();
}
return total;
}
Mention Etudiant::mention() const
{
return mentionPour(moyenne());
}
} // namespace uin
