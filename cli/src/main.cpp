/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

//#include <csignal>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

using namespace ftxui;

int main() {
    //signal(SIGPIPE, SIG_IGN);

    // Define the document
    Element document =
            hbox({
                         text("left") | border,
                         text("middle") | border | flex,
                         text("right") | border,
                 });

    auto screen = Screen::Create(
            Dimension::Full(),       // Width
            Dimension::Fit(document) // Height
    );
    Render(screen, document);
    screen.Print();

    return EXIT_SUCCESS;
}
