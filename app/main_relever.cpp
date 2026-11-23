#include "uincampus/uincampus.hpp"
#include <cstdlib>
#include <exception>
#include <format>
#include <print>
#include <span>
#include <string>
#include <string_view>
namespace {
enum class Suite { Continuer, Terminer, Erreur };

// afficherAide et traiterOptions : inchangées (chapitre 6)
Suite
traiterOptions(std::span<char*> args) {
return Suite::Continuer;
}
void afficherReleve(const uin::Etudiant& etudiant)
{
std::println("\n{:=<72}", "");
std::println("Relevé de notes — {} ({})", etudiant.nomComplet(), etudiant.matricule());
std::println("{:=<72}", "");
std::println("{:<8} {:<30} {:>5} {:>8} {:>6} {}","Code", "Intitulé", "Coef", "Crédits", "Note", "Statut");
std::println("{:-<72}", "");
for (const auto& [ue, note] : etudiant.resultats()) {
std::println("{:<8} {:<30} {:>5} {:>8} {:>6.2f} {}",ue.code(), ue.intitule(), ue.coefficient(), ue.credits(),note.valeur(), note.estValidante() ? "Validée" : "Non validée");
}
std::println("{:-<72}", "");
if (etudiant.aDesResultats()) {
std::println("Moyenne : {:.2f} Crédits : {} / {} Mention : {}",uin::arrondir(etudiant.moyenne()),etudiant.creditsObtenus(), etudiant.creditsInscrits(),uin::libelle(etudiant.mention()));
}
}
[[nodiscard]] uin::UE saisirUE(int numero)
{
std::println("\nUE {}", numero);
// Dans une liste entre accolades, les arguments sont évalués de gauche à droite :
// les questions sont donc posées dans l'ordre.
return uin::UE{
uin::lireTexteNonVide(" Code : "),
uin::lireTexteNonVide(" Intitulé : "),
uin::lireEntier(" Crédits (1 à 30) : ", 1, 30),
static_cast<double>(uin::lireEntier(" Coefficient (1 à 10) : ", 1, 10)),
};
}
} // namespace
int main(int argc, char* argv[])
{
switch (traiterOptions(std::span{argv, static_cast<std::size_t>(argc)})) {
case Suite::Terminer: return EXIT_SUCCESS;
case Suite::Erreur:
return EXIT_FAILURE;
case Suite::Continuer: break;
}
try {
std::println("UIN-Campus {} — Relevé d'un étudiant\n", uin::version());
const std::string matricule = uin::lireTexteNonVide("Matricule : ");
const std::string nom = uin::lireTexteNonVide("Nom : ");
const std::string prenom = uin::lireTexteNonVide("Prénom : ");
uin::Etudiant etudiant{matricule, nom, prenom};
const int nbUE = uin::lireEntier("Nombre d'UE (1 à 30) : ", 1, 30);
for (int i{1}; i <= nbUE; ++i) {
const uin::UE ue = saisirUE(i);
etudiant.enregistrerResultat(ue, uin::Note{uin::lireNote(" Note : ")});
}
afficherReleve(etudiant);
} catch (const std::exception& e) {
std::println(stderr, "Erreur : {}", e.what());
return EXIT_FAILURE;
}
return EXIT_SUCCESS;
}
