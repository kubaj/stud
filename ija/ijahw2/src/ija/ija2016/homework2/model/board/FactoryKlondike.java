package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;

public class FactoryKlondike extends AbstractFactorySolitaire {

    @Override
    public Card createCard(Card.Color color, int value) {
        if (value > 13 || value < 1) return null;
        return new Card(color, value);
    }

    @Override
    public CardDeck createCardDeck() {
        return CardDeck.createStandardDeck();
    }

    @Override
    public CardDeck createTargetPack(Card.Color color) {
        return null;
    }

    @Override
    public CardStack createWorkingPack() {
        return null;
    }
}