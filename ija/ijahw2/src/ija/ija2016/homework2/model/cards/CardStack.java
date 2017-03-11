package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by xkulic03 on 2/24/17.
 */
public class CardStack {

    private int size;
    ArrayList<Card> cards;

    public CardStack(int size) {
        this.size = size;
        this.cards = new ArrayList<Card>();
    }

    public boolean isEmpty() {
        return cards.size() == 0;
    }

    public void put(Card card) {
        if (cards.size() == this.size) {
            return;
        }
        this.cards.add(card);
    }

    public void put(CardStack stack) {
        cards.addAll(stack.getCards());
    }

    public ArrayList<Card> getCards() {
        return this.cards;
    }

    public int size() {
        return cards.size();
    }

    public CardStack takeFrom(Card card) {
        int cardIndex = this.cards.indexOf(card);
        CardStack cardStack = new CardStack(this.cards.size());
        cardStack.cards = new ArrayList<>(this.cards.subList(cardIndex, this.cards.size()));
        for (; cardIndex < this.cards.size(); ) {
            this.cards.remove(cardIndex);
        }
        return cardStack;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        CardStack cardStack = (CardStack) o;

        return cards != null ? cards.equals(cardStack.cards) : cardStack.cards == null;
    }

    @Override
    public int hashCode() {
        return cards != null ? cards.hashCode() : 0;
    }
}
