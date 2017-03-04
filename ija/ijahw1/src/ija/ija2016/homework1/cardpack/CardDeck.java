package ija.ija2016.homework1.cardpack;

import java.util.Arrays;

/**
 * Created by xkulic03 on 2/24/17.
 */
public class CardDeck {

    private Card[] cardDeck;
    private int top;
    private int size;

    public CardDeck(int size) {
        this.cardDeck = new Card[size];
        this.top = -1;
        this.size = size;
    }

    public static CardDeck createStandardDeck() {
        CardDeck cd = new CardDeck(52);
        for (Card.Color c: Card.Color.values()) {
            for (int i = 1; i <= 13; i++) {
                cd.put(new Card(c, i));
            }
        }

        return cd;
    }

    public int size() {
        return this.top + 1;
    }

    public void put(Card card) {

        if (top + 1 == size) {
            return;
        }

        this.cardDeck[++top] = card;
    }

    public Card pop() {

        if (top == -1) {
            return null;
        }

        return this.cardDeck[top--];
    }
}
