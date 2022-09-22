from django import forms

from core.models import Player


class LinkForm(forms.Form):

    link_token = forms.CharField()


class GameForm(forms.Form):

    BLACK = 'black'
    WHITE = 'white'
    COLOR_CHOICES = [
        (BLACK, 'Black'),
        (WHITE, 'White'),
    ]

    def __init__(self, *args, player=None, **kwargs):
        super().__init__(*args, **kwargs)
        if player:
            self.player = player
            self.fields['opponent'].queryset = Player.objects.all().exclude(pk=player.pk)

    opponent = forms.ModelChoiceField(Player.objects.none())
    color = forms.ChoiceField(choices=COLOR_CHOICES, label="I'd like to play...")


class UsernameForm(forms.ModelForm):

    class Meta:
        
        model = Player
        fields = ['username']


class RegistrationForm(forms.ModelForm):

    email = forms.EmailField()
    password = forms.CharField(widget=forms.PasswordInput)

    class Meta:
        
        model = Player
        fields = ['username']
